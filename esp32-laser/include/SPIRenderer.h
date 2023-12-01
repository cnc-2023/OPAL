#include <Arduino.h>
#include <SPI.h>
#include "driver/spi_master.h"
#include "config.h"

typedef struct spi_device_t *spi_device_handle_t; ///< Handle for a device on a SPI bus

class SPIRenderer
{
private:
    TaskHandle_t spi_task_handle;
    spi_device_handle_t spi;
    volatile int draw_position;
    volatile int frame_position;

public:
    SPIRenderer();
    void IRAM_ATTR Draw();
    void Start();
    void IRAM_ATTR SetPosition(int x, int y);
    // friend void spi_draw_timer(void *para);
};
