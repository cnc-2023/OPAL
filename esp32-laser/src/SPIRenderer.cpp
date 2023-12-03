#include <SPIRenderer.h>

SPIRenderer::SPIRenderer()
{
    frame_position = 0;
    draw_position = 0;
}

void IRAM_ATTR SPIRenderer::Draw()
{
}

void IRAM_ATTR SPIRenderer::SetPosition(int x, int y)
{
    // const ILDA_Record_t &instruction = ilda->frames[frame_position].records[draw_position];
    // int y = 2048 + (instruction.x * 1024) / 32768;
    // int x = 2048 + (instruction.y * 1024) / 32768;
    esp_err_t ret;

    spi_transaction_t t1 = {};
    t1.length = 16;
    t1.flags = SPI_TRANS_USE_TXDATA;
    t1.tx_data[0] = 0b11010000 | ((x >> 8) & 0xF);
    t1.tx_data[1] = x & 255;
    ret = spi_device_polling_transmit(spi, &t1);
    // printf("Set X Ret code is %d\n", ret);
    // channel B
    spi_transaction_t t2 = {};
    t2.length = 16;
    t2.flags = SPI_TRANS_USE_TXDATA;
    t2.tx_data[0] = 0b01010000 | ((y >> 8) & 0xF);
    t2.tx_data[1] = y & 255;
    ret = spi_device_polling_transmit(spi, &t2);
    // printf("Set Y Ret code is %d\n", ret);

    // DAC Load
    digitalWrite(PIN_NUM_LDAC, LOW);
    digitalWrite(PIN_NUM_LDAC, HIGH);
}

void SPIRenderer::Start()
{

    pinMode(PIN_NUM_LASER_R, OUTPUT);
    pinMode(PIN_NUM_LASER_G, OUTPUT);
    pinMode(PIN_NUM_LASER_B, OUTPUT);
    pinMode(PIN_NUM_LDAC, OUTPUT);

    // setup SPI output
    esp_err_t ret;
    spi_bus_config_t buscfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 0};
    spi_device_interface_config_t devcfg = {
        .command_bits = 0,
        .address_bits = 0,
        .dummy_bits = 0,
        .mode = 0,
        .clock_speed_hz = 40000000,
        .spics_io_num = PIN_NUM_CS, // CS pin
        .flags = SPI_DEVICE_NO_DUMMY,
        .queue_size = 2,
    };
    // Initialize the SPI bus
    ret = spi_bus_initialize(HSPI_HOST, &buscfg, 1);
    printf("spi_bus_initialize Ret: %d\n", ret);
    ret = spi_bus_add_device(HSPI_HOST, &devcfg, &spi);
    printf("spi_bus_add_device Ret: %d\n", ret);

    // xTaskCreatePinnedToCore(
    //     fileBufferLoop, "fileBufferHandle", 5000 // Stack size
    //     ,
    //     NULL, 3 // Priority
    //     ,
    //     &fileBufferHandle, 0);
}
