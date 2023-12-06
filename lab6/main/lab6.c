#include <stdio.h>
#include "sdkconfig.h"
#include "esp_system.h"

// Libraries needed for Network
#include "wifi.h"

void app_main(void)
{
	// Cannot run both functions at the same time
	// If you want to, modify source code to make ESP32 wifi run in APSTA mode

	create_wifi();
	// connect_wifi();
}
