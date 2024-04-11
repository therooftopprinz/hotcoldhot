#include "storage_manager.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "esp_flash.h"
#include "esp_flash_spi_init.h"
#include "esp_partition.h"
#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "esp_system.h"

#include "config.h"

// #define PIN_NUM_MOSI 23
// #define PIN_NUM_MISO 19
// #define PIN_NUM_CLK  18
// #define PIN_NUM_CS    5

static const char *TAG = "storage_manager";

void storage_manager_deinit()
{
}

void storage_manager_init(void)
{
    esp_vfs_fat_sdmmc_mount_config_t mount_config =
        {
            .format_if_mount_failed = true,
            .max_files = 5,
            .allocation_unit_size = 16 * 1024
        };

    sdmmc_card_t *card;
    const char mount_point[] = "/sdcard";
    ESP_LOGI(TAG, "Initializing SD card");
    ESP_LOGI(TAG, "Using SPI peripheral");

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot = SPI3_HOST;

    spi_bus_config_t bus_cfg =
        {
            .mosi_io_num = PIN_NUM_MOSI,
            .miso_io_num = PIN_NUM_MISO,
            .sclk_io_num = PIN_NUM_CLK,
            .quadwp_io_num = -1,
            .quadhd_io_num = -1,
            .max_transfer_sz = 4000,
        };

    // const spi_bus_config_t spi_bus_config = {
    //     .mosi_io_num = ST7796_SPI_BUS_MOSI_IO_NUM,
    //     .miso_io_num = ST7796_SPI_BUS_MISO_IO_NUM,
    //     .sclk_io_num = ST7796_SPI_BUS_SCLK_IO_NUM,
    //     .quadwp_io_num = ST7796_SPI_BUS_QUADWP_IO_NUM,
    //     .quadhd_io_num = ST7796_SPI_BUS_QUADHD_IO_NUM,
    //     .max_transfer_sz = ST7796_SPI_BUS_MAX_TRANSFER_SZ,
    //     .flags = ST7796_SPI_BUS_FLAGS,
    //     .intr_flags = ST7796_SPI_BUS_INTR_FLAGS};
    // ESP_ERROR_CHECK_WITHOUT_ABORT(spi_bus_initialize(ST7796_SPI_HOST, &spi_bus_config, ST7796_SPI_DMA_CHANNEL));


    auto ret = spi_bus_initialize(host.slot, &bus_cfg, SDSPI_DEFAULT_DMA);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize bus.");
        return;
    }

    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = PIN_NUM_CS;
    slot_config.host_id = host.slot;

    ESP_LOGI(TAG, "Mounting filesystem");
    ret = esp_vfs_fat_sdspi_mount(mount_point, &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            ESP_LOGE(TAG, "Failed to mount filesystem.");
        }
        else
        {
            ESP_LOGE(TAG, "Failed to initialize the card (%s).", esp_err_to_name(ret));
        }
        return;
    }

    ESP_LOGI(TAG, "Filesystem mounted");

    // Card has been initialized, print its properties
    sdmmc_card_print_info(stdout, card);
}
