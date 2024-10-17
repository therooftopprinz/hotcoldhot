#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_system.h"
#include "esp_log.h"

#include "owb.h"
#include "owb_rmt.h"
#include "ds18b20.h"
#include <cstring>
#include <ctime>
#include <cmath>
#include <thread>

#include "board_devices.hpp"

#define GPIO_DS18B20_0       (CONFIG_ONE_WIRE_GPIO)
#define MAX_DEVICES          (8)
#define DS18B20_RESOLUTION   (DS18B20_RESOLUTION_10_BIT)
#define SAMPLE_PERIOD        (187.5)   // usec

#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO          (14) // Define the output GPIO
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_RES           LEDC_TIMER_8_BIT // Set duty resolution to 13 bits
#define LEDC_FREQUENCY          (100) // Frequency in Hertz. Set frequency at 4 kHz

float temp;

static int64_t time_us()
{
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

static void run()
{
    temp = 0;
    while (true)
    {
        vTaskDelay(2000.0 / portTICK_PERIOD_MS);

        OneWireBus * owb;
        owb_rmt_driver_info rmt_driver_info;
        owb = owb_rmt_initialize(&rmt_driver_info, (gpio_num_t)GPIO_DS18B20_0, RMT_CHANNEL_1, RMT_CHANNEL_0);
        owb_use_crc(owb, true);

        printf("Find devices:\n");
        OneWireBus_ROMCode device_rom_codes[MAX_DEVICES];
        memset(device_rom_codes, 0, sizeof(device_rom_codes));

        int num_devices = 0;
        OneWireBus_SearchState search_state;
        memset(&search_state, 0, sizeof(search_state));
        bool found = false;
        owb_search_first(owb, &search_state, &found);
        while (found)
        {
            char rom_code_s[17];
            owb_string_from_rom_code(search_state.rom_code, (char*) device_rom_codes, sizeof(rom_code_s));
            printf("  %d : %s\n", num_devices, rom_code_s);
            device_rom_codes[num_devices] = search_state.rom_code;
            ++num_devices;
            owb_search_next(owb, &search_state, &found);
        }
        printf("Found %d device%s\n", num_devices, num_devices == 1 ? "" : "s");

        // if (num_devices == 1)
        // {
        //     // For a single device only:
        //     OneWireBus_ROMCode rom_code;
        //     owb_status status = owb_read_rom(owb, &rom_code);
        //     if (status == OWB_STATUS_OK)
        //     {
        //         char rom_code_s[OWB_ROM_CODE_STRING_LENGTH];
        //         owb_string_from_rom_code(rom_code, rom_code_s, sizeof(rom_code_s));
        //         printf("Single device %s present\n", rom_code_s);
        //     }
        //     else
        //     {
        //         printf("An error occurred reading ROM code: %d", status);
        //     }
        // }
        // else
        {
            // Search for a known ROM code (LSB first):
            // For example: 0x1502162ca5b2ee28[
            OneWireBus_ROMCode known_device;
            known_device.fields.family[0] = 0x28;
            memcpy(known_device.fields.serial_number, "\xee\xb2\xa5\x2c\x16\x02", 6);
            known_device.fields.crc[0] = 0x15;

            char rom_code_s[OWB_ROM_CODE_STRING_LENGTH];
            owb_string_from_rom_code(known_device, rom_code_s, sizeof(rom_code_s));
            bool is_present = false;

            owb_status search_status = owb_verify_rom(owb, known_device, &is_present);
            if (search_status == OWB_STATUS_OK)
            {
                printf("Device %s is %s\n", rom_code_s, is_present ? "present" : "not present");
            }
            else
            {
                printf("An error occurred searching for known device: %d", search_status);
            }
        }

        DS18B20_Info* ds18b20_info = ds18b20_malloc();  // heap allocation
        ds18b20_init(ds18b20_info, owb, device_rom_codes[0]); // associate with bus and device
        ds18b20_use_crc(ds18b20_info, true);           // enable CRC check on all reads
        ds18b20_set_resolution(ds18b20_info, DS18B20_RESOLUTION);


        // Read temperatures more efficiently by starting conversions on all devices at the same time
        int rv = 0;
        int errors = 0;

        if (num_devices > 0)
        {
            TickType_t last_wake_time = xTaskGetTickCount();

            while (1)
            {
                ds18b20_convert_all(owb);
                ds18b20_wait_for_conversion(ds18b20_info);

                rv = ds18b20_read_temp(ds18b20_info, &temp);
                if (rv != DS18B20_OK)
                {
                    errors += rv;
                    printf("errors %d", errors);
                    continue;
                }

                unsigned long t = time(NULL);
                printf("%lld,%.1f,%d|\n", time_us(), temp, errors);

                vTaskDelayUntil(&last_wake_time, SAMPLE_PERIOD / portTICK_PERIOD_MS);
            }
        }
        else
        {
            printf("\nNo DS18B20 devices detected!\n");
        }

        ds18b20_free(&ds18b20_info);
        owb_uninitialize(owb);

        fflush(stdout);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

std::thread runner;

#define PIN_CTL 12

board_devices::board_devices()
{
    runner = std::thread([](){run();});

    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ull << PIN_CTL);
    io_conf.pull_down_en = (gpio_pulldown_t) 0;
    io_conf.pull_up_en = (gpio_pullup_t) 0;
    gpio_config(&io_conf);

    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {};
    ledc_timer.speed_mode       = LEDC_MODE;
    ledc_timer.timer_num        = LEDC_TIMER;
    ledc_timer.duty_resolution  = LEDC_DUTY_RES;
    ledc_timer.freq_hz          = LEDC_FREQUENCY;  // Set output frequency at 4 kHz
    ledc_timer.clk_cfg          = LEDC_AUTO_CLK;
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {};
    ledc_channel.speed_mode     = LEDC_MODE;
    ledc_channel.channel        = LEDC_CHANNEL;
    ledc_channel.timer_sel      = LEDC_TIMER;
    ledc_channel.intr_type      = LEDC_INTR_DISABLE;
    ledc_channel.gpio_num       = LEDC_OUTPUT_IO;
    ledc_channel.duty           = 0; // Set duty to 0%
    ledc_channel.hpoint         = 0;
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, 0));

    gpio_set_level(gpio_num_t(1ull<<PIN_CTL), 0);
}

int64_t board_devices::time_us()
{
    return ::time_us();
}

void board_devices::set_peltier_duty_100(float d)
{
    d *= -1;
    uint8_t dr = fabs(d/100)*255;
    uint8_t g = 0;
    if (d < 0)
    {
        dr = 255-dr;
        g = 1;
    }    

    printf("pwm %d %d\n", dr, g);
    gpio_set_level(gpio_num_t(PIN_CTL), g);
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, dr));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
}

float board_devices::get_sensor_temp()
{
    return temp;
}

void board_devices::loop()
{}
