#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"
#include "color.h"
#include "hittable.h"

#include <iostream>

/* Two main functions:
   1. Construct and dispatch rays into the world.
   2. Use the results of these rays to construct the rendered image.
*/

class camera {
    public:
    
    // Aspect ratio
    double aspect_ratio = 16.0 / 9.0; // image width over height ratio.
    int image_width = 800; // Rendered image width in pixel count.

    void render(const hittable &world) {
        initialize();

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; ++j)
        {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; ++i)
            {
                auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
                auto ray_direction = pixel_center - camera_center;
                ray r(camera_center, ray_direction);

                color pixel_color = ray_color(r, world);
                write_color(std::cout, pixel_color);
            }
        
        }
        std::clog << "\rDone.              \n";
    }

    private:
    int image_height;
    point3 camera_center;
    point3 pixel00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;

    void initialize(){
        // Calculate image height. We need to turn operation to int again
        image_height = static_cast<int>(image_width / aspect_ratio);

        //Make the image height at least 1 pixel.
        image_height = (image_height < 1) ? 1 : image_height;
        
        // Render viewport (from where we will be looking at the 3D space) 
        // We are using real ratio instead of ideal ratio.

        auto viewport_height = 2.0;
        auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);

        // Camera center
        camera_center = point3(0, 0, 0);
        auto focal_length = 1.0;

        // Position to start rendering the view
        auto viewport_u = vec3(viewport_width, 0, 0);
        auto viewport_v = vec3(0, -viewport_height, 0);

        // Pixel Delta Vectors 
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left = camera_center
                                - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    color ray_color(const ray& r, const hittable& world) const {
        hit_record rec;
        if (world.hit(r, interval(0, infinity), rec)) {
            return 0.5 * (rec.normal + color(1,1,1));
        }

        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5*(unit_direction.y() + 1.0);
        return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
    }
};

#endif
