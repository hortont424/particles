// Not covered by toplevel license.

// Borrowed from:
// http://www.cocoadev.com/index.pl?NSPointFunctions

#import <Cocoa/Cocoa.h>
#import <math.h>

extern const NSPoint NSFarAwayPoint;

static inline NSPoint NSAddPoints(NSPoint firstPoint, NSPoint secondPoint)
{
	return NSMakePoint(firstPoint.x+secondPoint.x, firstPoint.y+secondPoint.y);
}

static inline NSPoint NSSubtractPoints(NSPoint firstPoint, NSPoint secondPoint)
{
	return NSMakePoint(firstPoint.x-secondPoint.x, firstPoint.y-secondPoint.y);
}

static inline NSPoint NSCartesianToPolar(NSPoint cartesianPoint)
{
    return NSMakePoint(sqrtf(cartesianPoint.x * cartesianPoint.x +
                             cartesianPoint.y * cartesianPoint.y),
                       atan2f(cartesianPoint.y, cartesianPoint.x));
}

static inline NSPoint NSPolarToCartesian(NSPoint polarPoint)
{
    return NSMakePoint(polarPoint.x * cosf(polarPoint.y),
                       polarPoint.x * sinf(polarPoint.y));
}

// Everything below is covered under toplevel license and written by me

static inline NSRect NSRectAroundPoint(NSPoint point, CGFloat x, CGFloat y)
{
    return NSInsetRect(NSMakeRect(point.x, point.y, 0.0, 0.0), -x, -y);
}

static inline CGRect CGRectAroundPoint(NSPoint point, CGFloat x, CGFloat y)
{
    return CGRectInset(CGRectMake(point.x, point.y, 0.0, 0.0), -x, -y);
}

static inline BOOL NSComparePoint(NSPoint a, NSPoint b)
{
    return a.x == b.x && a.y == b.y;
}

static inline NSRect NSRectWithPoints(NSPoint a, NSPoint b)
{
    NSRect r;

    if(a.x < b.x)
    {
        r.origin.x = a.x;
        r.size.width = b.x - a.x;
    }
    else
    {
        r.origin.x = b.x;
        r.size.width = a.x - b.x;
    }

    if(a.y < b.y)
    {
        r.origin.y = a.y;
        r.size.height = b.y - a.y;
    }
    else
    {
        r.origin.y = b.y;
        r.size.height = a.y - b.y;
    }

    return r;
}
