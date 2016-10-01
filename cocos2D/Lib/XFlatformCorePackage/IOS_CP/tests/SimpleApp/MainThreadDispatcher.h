#ifndef __MAINTHREADDISPATCHER_H__
#define __MAINTHREADDISPATCHER_H__

#include <functional>

extern void RunOnUiThread(std::function<void()>);

#endif