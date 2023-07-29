#include "Camera.h"
#include "Group.h"
#include "image.h"
#include "light.h"
#include "ray.h"
#include "scene_parser.h"
#include "glCanvas.h"
#include <GL/glut.h>
#include <assert.h>
#include <cstring>
#include <iostream>
using namespace std;

char *input_file = nullptr;
int width = 100;
int height = 100;
char *output_file = nullptr;
float depth_min = 0;
float depth_max = 1;
char *depth_file = nullptr;
char *normal_file = nullptr;
bool shade_back = false;
bool gui = false;
int theta_steps, phi_steps;
bool gouraud = false;

void renderFun(void) {
    SceneParser parser = SceneParser(input_file);
    Image img = Image(width, height);
    Image gray_img = Image(width, height);
    Image normal_img = Image(width, height);

    Vec3f color = parser.getBackgroundColor();
    Group* objs = parser.getGroup();
    Camera* c = parser.getCamera();
    int light_num = parser.getNumLights();
    Vec3f ambient_color = parser.getAmbientLight();
    gray_img.SetAllPixels(Vec3f(0, 0, 0));
    img.SetAllPixels(color);

    int size = width < height ? width : height;
    float step = 1. / size;
    Hit h;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            float x = i * step;
            float y = j * step;
            Ray r = c -> generateRay(Vec2f(x, y));
            h.set(3e7, NULL, Vec3f(0, 0, 0), Ray());
            bool intersected = objs -> intersect(r, h, c -> getTMin());
            if (intersected) {
                /* Phong shading */
                assert (h.getMaterial() != NULL);
                Vec3f direction, light_color;
                Vec3f object_color = h.getMaterial() -> getDiffuseColor();
                Vec3f pixel_color = ambient_color * object_color;
                Light* temp_light;
                for (int l = 0; l < light_num; ++l) {
                    temp_light = parser.getLight(l);
                    float dummy_dist;
                    temp_light -> getIllumination(h.getIntersectionPoint(), direction, light_color, dummy_dist);
                    Vec3f phong_color = h.getMaterial() -> Shade(r, h, direction, light_color);
                    pixel_color += phong_color;
                }
                img.SetPixel(i, j, pixel_color);

                /* depth shading */
                if (depth_file != NULL) {
                    float depth = h.getT();
                    if (depth < depth_min) depth = depth_min;
                    else if (depth > depth_max) depth = depth_max;
                    float gray_color = 1 - (depth - depth_min) / (depth_max - depth_min);
                    gray_img.SetPixel(i, j, Vec3f(gray_color, gray_color, gray_color));
                }

                /* normal visualization */
                if (normal_file != NULL) {
                    Vec3f normal_color = h.getNormal();
                    normal_img.SetPixel(i, j, Vec3f(abs(normal_color.r()), abs(normal_color.g()), abs(normal_color.b())));
                }
            }
        }
    }
    img.SaveTGA(output_file);
    printf("SaveTGA done\n");
    if (depth_file != NULL) gray_img.SaveTGA(depth_file);
    if (normal_file != NULL) normal_img.SaveTGA(normal_file);
}

// // 声明渲染函数
// void renderFun()
// {
//     // 在这里实现光线追踪代码
//     // 创建和解析场景
//     SceneParser parse = SceneParser(input_file);
//     // 创建图像缓冲区
//     int size = width < height ? width : height;
//     Image image(size, size);
//     Image imageDepth(size, size);
//     Image imageNormal = Image(size, size);
//     // 初始化图像颜色
//     image.SetAllPixels(parse.getBackgroundColor());
//     imageDepth.SetAllPixels(Vec3f(0, 0, 0));

//     // 创建相机
//     Camera *camera = parse.getCamera();

//     // 创建场景物品
//     Group *group = parse.getGroup();

//     // 遍历像素，生成射线并与场景相交
//     for (int y = 0; y < size; y++)
//     {
//         for (int x = 0; x < size; x++)
//         {
//             // 将像素坐标归一化到 [0, 1]
//             Vec2f point(float(x) / size, float(y) / size);
//             // 生成射线
//             Ray ray = camera->generateRay(point);
//             // 创建一个空的 Hit 对象
//             Hit hit;
//             hit.set(1e10, NULL, Vec3f(0, 0, 0), Ray());
//             // 与场景进行相交
//             if (group->intersect(ray, hit, camera->getTMin()))
//             {
//                 // 直接着色
//                 // // 将最近相交对象的颜色写入图像
//                 // // 使用环境光计算击中对象的颜色
//                 // assert(hit.getMaterial() != NULL);
//                 // Vec3f objectColor = hit.getMaterial()->getDiffuseColor();
//                 // Vec3f pixelColor = parse.getAmbientLight() * objectColor;

//                 // // 添加来自每一个光源的贡献
//                 // for (int i = 0; i < parse.getNumLights(); i++)
//                 // {
//                 //     Light *light = parse.getLight(i);
//                 //     Vec3f lightDir, lightColor;
//                 //     // 从光源获取照明信息
//                 //     light->getIllumination(hit.getIntersectionPoint(), lightDir, lightColor);
//                 //     // 计算光源方向和表面法线的点积
//                 //     float diffFactor = lightDir.Dot3(hit.getNormal());
//                 //     if (diffFactor < 0)
//                 //     {
//                 //         // cout << "diffFactor < 0 " << "(" << x << "," << y << ")" << endl;
//                 //         if (shade_back)
//                 //         {
//                 //             // cout << "shade_back" << " " << "diffFactor = " << diffFactor << endl;
//                 //             diffFactor = -diffFactor;
//                 //         }
//                 //         else
//                 //             diffFactor = 0;
//                 //     }
//                 //     // 将漫反射分量添加到像素颜色中
//                 //     pixelColor += diffFactor * lightColor * objectColor;
//                 // }
//                 // image.SetPixel(x, y, pixelColor);

//                 // Phong 着色
//                 assert(hit.getMaterial() != NULL);
//                 Vec3f direction, light_color;
//                 Vec3f object_color = hit.getMaterial()->getDiffuseColor();
//                 Vec3f pixel_color = parse.getAmbientLight() * object_color;
//                 Light *temp_light;
//                 for (int l = 0; l < parse.getNumLights(); ++l)
//                 {
//                     temp_light = parse.getLight(l);
//                     float dummy_dist;
//                     temp_light->getIllumination(hit.getIntersectionPoint(), direction, light_color, dummy_dist);
//                     Vec3f phong_color = hit.getMaterial()->Shade(ray, hit, direction, light_color);
//                     pixel_color += phong_color;
//                 }
//                 image.SetPixel(x, y, pixel_color);

//                 // 渲染深度图像
//                 if (depth_file != NULL)
//                 {
//                     // 限制深度值在 [minDepth, maxDepth]
//                     float t = hit.getT();
//                     t = std::max(depth_min, std::min(depth_max, t));
//                     // 将深度映射为0到1之间的灰度值
//                     float grayscale = 1 - (t - depth_min) / (depth_max - depth_min);
//                     // 使用灰度值来设置颜色
//                     imageDepth.SetPixel(x, y, Vec3f(grayscale, grayscale, grayscale));
//                 }

//                 // 渲染法向量图像
//                 if (normal_file != nullptr)
//                 {
//                     // 将法向量归一化到
//                     Vec3f normal = hit.getNormal();
//                     normal = Vec3f(abs(normal.x()), abs(normal.y()), abs(normal.z()));
//                     // 使用法向量来设置颜色
//                     imageNormal.SetPixel(x, y, normal);
//                 }
//             }
//             else
//             {
//                 imageDepth.SetPixel(x, y, Vec3f(0, 0, 0));
//             }
//         }
//     }

//     // 保存图像
//     image.SaveTGA(output_file);
//     if (depth_file != NULL)
//         imageDepth.SaveTGA(depth_file);
//     if (normal_file != NULL)
//         imageNormal.SaveTGA(normal_file);
// }

int main(int argc, char *argv[])
{
    // ========================================================
    // ========================================================
    // Some sample code you might like to use for parsing
    // command line arguments

    // sample command line:
    // raytracer -input scene1_1.txt -size 200 200 -output output1_1.tga -depth 9 10 depth1_1.tga

    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-input"))
        {
            i++;
            assert(i < argc);
            input_file = argv[i];
        }
        else if (!strcmp(argv[i], "-size"))
        {
            i++;
            assert(i < argc);
            width = atoi(argv[i]);
            i++;
            assert(i < argc);
            height = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-output"))
        {
            i++;
            assert(i < argc);
            output_file = argv[i];
        }
        else if (!strcmp(argv[i], "-depth"))
        {
            i++;
            assert(i < argc);
            depth_min = atof(argv[i]);
            i++;
            assert(i < argc);
            depth_max = atof(argv[i]);
            i++;
            assert(i < argc);
            depth_file = argv[i];
        }
        else if (!strcmp(argv[i], "-normals"))
        {
            i++;
            assert(i < argc);
            normal_file = argv[i];
        }
        else if (!strcmp(argv[i], "-shade_back"))
        {
            shade_back = true;
        }
        else if (!strcmp(argv[i], "-gui"))
        {
            gui = true;
        }
        else if (!strcmp(argv[i], "-tessellation"))
        {
            i++;
            assert(i < argc);
            theta_steps = atoi(argv[i]);
            i++;
            assert(i < argc);
            phi_steps = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-gouraud"))
        {
            gouraud = true;
        }
        else
        {
            printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
            assert(0);
        }

    }

    // ========================================================
    // ========================================================

    // parse scene file
    SceneParser parse(input_file);
    if (gui)
    {
        // 创建一个GLCanvas实例
        GLCanvas canvas;
        glutInit(&argc, argv);
        // 初始化GLCanvas
        canvas.initialize(&parse, renderFun);
    }
    else{
        renderFun();
    }

}