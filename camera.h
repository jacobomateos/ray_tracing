#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"
#include "color.h"
#include "hittable.h"
#include "material.h"

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
    int samples_per_pixel = 10; //Count of random samples for each pixel.
    int max_depth = 10; // Max number of ray bounces into scene.

    void render(const hittable &world) {
        initialize();

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; ++j)
        {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; ++i)
            {
                color pixel_color(0,0,0);
                for (int sample = 0; sample < samples_per_pixel; ++sample){
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                write_color(std::cout, pixel_color, samples_per_pixel);
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

    color ray_color(const ray& r, int depth, const hittable& world) const {
        hit_record rec;

        
        //If we've exceeded the ray bount limit, no more light is gathered.
        if (depth <= 0)
            return color(0,0,0);


        if (world.hit(r, interval(0.0001, infinity), rec)) {
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered)){
                return attenuation * ray_color(scattered, depth-1, world);
            }
            return color(0,0,0);
        }

        // Background color if no object is hit.
        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5*(unit_direction.y() + 1.0);
        return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
    }

    ray get_ray(int i, int j) const {
        // Get a random sampled camera ray for the pixel at location i, j.
        auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
        auto pixel_sample = pixel_center + pixel_sample_square();

        auto ray_origin = camera_center;
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    vec3 pixel_sample_square() const {
        // Return a random point in the square surrounding a pixel at the origen.
        auto px = -0.5 + random_double();
        auto py = -0.5 + random_double();
        return (px * pixel_delta_u)+(py * pixel_delta_v);
    }
};

#endif

