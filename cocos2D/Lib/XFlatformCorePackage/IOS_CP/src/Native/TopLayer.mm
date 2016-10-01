#include "IOSNative/TopLayer.h"
#import <objc/runtime.h>

#define TOP_LAYER_TAG 1000
#define Z_SHIFT 16
#define Z_MASK 0xFFFF

static UIView* container = nil;

// Original didAddSubview
typedef void (*didAddSubview_t)(id self, SEL cmd, UIView* subView);
static didAddSubview_t originalDidAddSubview = nil;
/////////////////////////

// Our hook
static void DidAddSubview(id self, SEL cmd, UIView* subView)
{
	if (originalDidAddSubview != nil)
		originalDidAddSubview(self, cmd, subView);

	if (self == container)
		[TopLayer Refresh];
}

@implementation TopLayer

+(void) SetContainer:(UIView*)theContainer
{
	container = theContainer;

	if (originalDidAddSubview != nil)
		return; // ignore. we've already made the hook
	
	originalDidAddSubview = (didAddSubview_t)class_replaceMethod([theContainer class], @selector(didAddSubview:), (IMP)DidAddSubview, "v@:@");
}

+(void) Present:(UIView*)view withZ:(ViewZPosition)zPosition
{
	view.tag = (TOP_LAYER_TAG << Z_SHIFT) | zPosition;
}

+(void) Dismiss:(UIView*)view
{
	view.tag = 0;
}

+(void) Refresh
{
	if (container == nil)
		return;

	// Select all the topViews
	NSMutableArray *topViews = [NSMutableArray array];
	for (UIView *view in container.subviews)
	{
		if ((view.tag >> Z_SHIFT) == TOP_LAYER_TAG)
			[topViews addObject:view];
	}

	// A simple bubble sort (preserving the relative order of views with the same Z) should be enough
	int nrTopViews = (int)topViews.count;

	for (int i = 0; i < nrTopViews  - 1; i++)
	{
		UIView* viewI = topViews[i];
		for (int j = i + 1; j < nrTopViews; j++)
		{
			UIView* viewJ = topViews[j];
			if ((viewI.tag & Z_MASK) > (viewJ.tag & Z_MASK))
			{
				topViews[j] = viewI;
				topViews[i] = viewJ;
				viewI = viewJ;
			}
		}
	}

	// Bring them to front
	for (UIView* topView in topViews)
		[container bringSubviewToFront:topView];
}
@end