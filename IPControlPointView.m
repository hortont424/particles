#import "IPControlPointView.h"

@implementation IPControlPointView

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    
    if (self)
    {

    }
    return self;
}

- (void)drawRect:(NSRect)dirtyRect
{
    [[NSColor whiteColor] setFill];
    NSRectFill(dirtyRect);
}

@end
