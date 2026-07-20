#include <mulle-geometry/mulle-geometry.h>
#include <stdio.h>

static void print_rect(char *label, CGRect rect)
{
    mulle_printf("%s: origin=(%.2f, %.2f), size=(%.2f, %.2f)\n",
           label,
           rect.origin.x,
           rect.origin.y,
           rect.size.width,
           rect.size.height);
}

int main() 
{
    CACircle circle;
    CAAngle startAngle;
    CAAngle endAngle;
    CGRect rect;

    // Test case 1: 0 to 90 degrees
    circle = CACircleMake(0.0f, 0.0f, 10.0f);
    startAngle = CADegreesToAngle(0.0f);
    endAngle = CADegreesToAngle(90.0f);
    rect = CACircleOuterRectOfSector(circle, startAngle, endAngle);
    print_rect("Sector 0-90", rect);

    // Test case 2: 45 to 135 degrees
    startAngle = CADegreesToAngle(45.0f);
    endAngle = CADegreesToAngle(135.0f);
    rect = CACircleOuterRectOfSector(circle, startAngle, endAngle);
    print_rect("Sector 45-135", rect);

    // Test case 3: 180 to 270 degrees
    startAngle = CADegreesToAngle(180.0f);
    endAngle = CADegreesToAngle(270.0f);
    rect = CACircleOuterRectOfSector(circle, startAngle, endAngle);
    print_rect("Sector 180-270", rect);

    // Test case 4: A full circle
    startAngle = CADegreesToAngle(0.0f);
    endAngle = CADegreesToAngle(360.0f);
    rect = CACircleOuterRectOfSector(circle, startAngle, endAngle);
    print_rect("Sector 0-360", rect);

    // Test case 5: Zero angle difference
    startAngle = CADegreesToAngle(45.0f);
    endAngle = CADegreesToAngle(45.0f);
    rect = CACircleOuterRectOfSector(circle, startAngle, endAngle);
    print_rect("Sector 45-45", rect);

    return 0;
}