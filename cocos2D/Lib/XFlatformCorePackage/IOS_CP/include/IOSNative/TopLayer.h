#ifndef __TOPLAYER_H__
#define __TOPLAYER_H__

#include "IOSNative/Constants.h"
#import <UIKit/UIKit.h>

@interface TopLayer : NSObject

// Set the game view. This should be called only once by the game
+(void) SetContainer:(UIView*)theContainer;

// Tag the view as a top view. A top view is always drawn in front of all the normal views
+(void) Present:(UIView*)view withZ:(ViewZPosition)zPosition;

// Untag the view (make it a normal view)
+(void) Dismiss:(UIView*)view;

// For internal use only (it's here to disable compiler warnings)
+(void) Refresh;

@end

#endif