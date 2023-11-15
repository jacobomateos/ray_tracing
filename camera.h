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

    double vertical_field_view = 90; // Vertical view angle (field of view)
    point3 lookfrom = point3(0, 0, -1); // Point camera is looking from.
    point3 lookat = point3(0, 0, 0); // Point camera is looking at.
    vec3 v_up = vec3(0, 1, 0); // Cmaera-relative "up" direction

    double defocus_angle = 0; // Variation angle of rays through each pixel.
    double focus_distance = 10; // Distance from camera lookfrom point to plane of perfect focus.

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
    point3 pixel00_loc; // Location of pixel (0, 0).
    vec3 pixel_delta_u; // Offset to pixel to right.
    vec3 pixel_delta_v; // Offset to pixel below.
    vec3 u, v, w; // Camera fram basis vectors.

    vec3 defocus_disk_u; // Defocus disk horizontal radius.
    vec3 defocus_disk_v; // Defocus disk vertical radius.

    void initialize(){
        // Calculate image height.
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;
        
        camera_center = lookfrom;
        
        // Camera center
        
        auto theta_field_view = degrees_to_radians(vertical_field_view);
        auto h = tan(theta_field_view/2);
        
        // Render viewport (from where we will be looking at the 3D space) 
        auto viewport_height = 2 * h * focus_distance;
        auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);

        //calculate the u, v, w unit basis vectors for the camera coordinate frame.
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(v_up, w));
        v = cross(w, u); 

        // Position to start rendering the view
        auto viewport_u = viewport_width * u;
        auto viewport_v = viewport_height * -v;

        // Pixel Delta Vectors 
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left = camera_center 
                                - (focus_distance * w) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        // Calculate the camera defocus disk basis vectors.
        auto defocus_radius = focus_distance * tan(degrees_to_radians(defocus_angle/2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
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

        auto ray_origin = (defocus_angle <= 0) ? camera_center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    point3 defocus_disk_sample() const {
        // Returns a random point in the camera defocus disk.
        auto p = random_in_unit_disk();
        return camera_center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    vec3 pixel_sample_square() const {
        // Return a random point in the square surrounding a pixel at the origen.
        auto px = -0.5 + random_double();
        auto py = -0.5 + random_double();
        return (px * pixel_delta_u)+(py * pixel_delta_v);
    }
};

#endif

