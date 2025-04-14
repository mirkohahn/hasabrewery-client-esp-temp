#ifndef BATTERY_MONITOR_H
#define BATTERY_MONITOR_H

#if ENABLE_BATTERY_MONITOR
void setupBatteryMonitor();
#endif

float get_battery_status();

#endif
