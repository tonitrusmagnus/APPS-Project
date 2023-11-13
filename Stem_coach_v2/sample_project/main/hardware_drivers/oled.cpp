#include "oled.hpp"


oled::oled(/* args */)
{
    init = 0;
}

oled::~oled()
{
}
void oled::settext(char* text,int row){
    if(init == 1){
        ssd1306_display_text(&dev,row,text,15,false);
	}
}

void oled::oled_init(){
    #if CONFIG_I2C_INTERFACE
	i2c_master_init(&dev, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
	#endif // CONFIG_I2C_INTERFACE
	#if CONFIG_SPI_INTERFACE
	spi_master_init(&dev, CONFIG_MOSI_GPIO, CONFIG_SCLK_GPIO, CONFIG_CS_GPIO, CONFIG_DC_GPIO, CONFIG_RESET_GPIO);
	#endif // CONFIG_SPI_INTERFACE
	#if CONFIG_FLIP
	dev._flip = true;
	#endif
	#if CONFIG_SSD1306_128x64
	ssd1306_init(&dev, 128, 64);
	#endif // CONFIG_SSD1306_128x64
	ssd1306_clear_screen(&dev, false);
	ssd1306_contrast(&dev, 0xff);
  	vTaskDelay(1000 / portTICK_PERIOD_MS);
    init = 1;
}