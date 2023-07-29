#include "scene_parser.h"
#include "image.h"
#include <cstring>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    // ========================================================
    // ========================================================
    // Some sample code you might like to use for parsing
    // command line arguments

    char *input_file = nullptr;
    int width = 100;
    int height = 100;
    char *output_file = nullptr;
    float depth_min = 0;
    float depth_max = 1;
    char *depth_file = nullptr;

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

    // 创建图像缓冲区
    int size = width < height ? width : height;
    Image image(size, size);
    Image imageDepth(size, size);
    image.SetAllPixels(parse.getBackgroundColor());
    imageDepth.SetAllPixels(Vec3f(0, 0, 0));

    // 创建相机
    Camera *camera = parse.getCamera();

    // 创建场景物品
    Group *group = parse.getGroup();

    // 遍历像素，生成射线并与场景相交
    for (int y = 0; y < size; y++)
    {
        for (int x = 0; x < size; x++)
        {
            // 将像素坐标归一化到 [0, 1]
            Vec2f point(float(x) / size, float(y) / size);
            // 生成射线
            Ray ray = camera->generateRay(point);
            // 创建一个空的 Hit 对象
            Hit hit;
            // 与场景进行相交
            if (group->intersect(ray, hit, camera->getTMin())){
                // 将最近相交对象的颜色写入图像
                image.SetPixel(x, y, hit.getMaterial()->getDiffuseColor());

                // 限制深度值在 [minDepth, maxDepth]
                float t = hit.getT();
                t = std::max(depth_min, std::min(depth_max, t));

                // 将深度映射为0到1之间的灰度值
                float grayscale = 1 - (t - depth_min) / (depth_max - depth_min);

                // 使用灰度值来设置颜色
                imageDepth.SetPixel(x, y, Vec3f(grayscale, grayscale, grayscale));
            }
            else{
                imageDepth.SetPixel(x, y, Vec3f(0, 0, 0));
            }
        }
    }

    // 保存图像
    image.SaveTGA(output_file);
    imageDepth.SaveTGA(depth_file);
}