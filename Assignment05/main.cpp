#include "Camera.h"
#include "Group.h"
#include "glCanvas.h"
#include "image.h"
#include "light.h"
#include "ray.h"
#include "scene_parser.h"
#include "RayTracer.h"
#include "grid.h"
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
bool shadows = false;
int bounces = 1;
float weight = 0.01;
bool visualize_grid = false;
int nx = 0, ny = 0, nz = 0;

void traceRayFunction(float x, float y)
{
    SceneParser parser = SceneParser(input_file);
    Camera *c = parser.getCamera();
    RayTracer tracer(&parser, bounces, weight);

    int size = width < height ? width : height;
    float step = 1. / size;
    Hit h;
    Ray r = c->generateRay(Vec2f(x, y));
    h.set(INFINITY, NULL, Vec3f(0, 0, 0), Ray());
    tracer.traceRay(r, c->getTMin(), 0, 1, 1, h);
    Hit h2;
    h2.set(INFINITY, NULL, Vec3f(0, 0, 0), Ray());
    tracer.getGrid()->intersect(r, h2, c->getTMin());
}

// 声明渲染函数
void renderFunction()
{
    // 在这里实现光线追踪代码
    // 创建和解析场景
    SceneParser parse = SceneParser(input_file);
    // 创建图像缓冲区
    int size = width < height ? width : height;
    Image image(size, size);
    Image imageDepth(size, size);
    Image imageNormal = Image(size, size);
    // 初始化图像颜色
    image.SetAllPixels(parse.getBackgroundColor());
    imageDepth.SetAllPixels(Vec3f(0, 0, 0));
    
    RayTracer tracer(&parse, bounces, weight);

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
            hit.set(INFINITY, NULL, Vec3f(0, 0, 0), Ray());
            Vec3f pixel_color = tracer.traceRay(ray, camera -> getTMin(), 0, 1, 1, hit);
            image.SetPixel(x, y, pixel_color);

            // 与场景进行相交
            // hit.set(INFINITY, NULL, Vec3f(0, 0, 0), Ray());
            // if (group->intersect(ray, hit, camera->getTMin()))
            // {
            //     // 渲染深度图像
            //     if (depth_file != NULL)
            //     {
            //         // 限制深度值在 [minDepth, maxDepth]
            //         float t = hit.getT();
            //         t = std::max(depth_min, std::min(depth_max, t));
            //         // 将深度映射为0到1之间的灰度值
            //         float grayscale = 1 - (t - depth_min) / (depth_max - depth_min);
            //         // 使用灰度值来设置颜色
            //         imageDepth.SetPixel(x, y, Vec3f(grayscale, grayscale, grayscale));
            //     }

            //     // 渲染法向量图像
            //     if (normal_file != nullptr)
            //     {
            //         // 将法向量归一化到
            //         Vec3f normal = hit.getNormal();
            //         normal = Vec3f(abs(normal.x()), abs(normal.y()), abs(normal.z()));
            //         // 使用法向量来设置颜色
            //         imageNormal.SetPixel(x, y, normal);
            //     }
            // }
            // else
            // {
            //     imageDepth.SetPixel(x, y, Vec3f(0, 0, 0));
            // }
        }
    }

    // 保存图像
    if (output_file != nullptr)
        image.SaveTGA(output_file);
    if (depth_file != NULL)
        imageDepth.SaveTGA(depth_file);
    if (normal_file != NULL)
        imageNormal.SaveTGA(normal_file);
}

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
        else if (!strcmp(argv[i], "-shadows"))
        {
            shadows = true;
        }
        else if (!strcmp(argv[i], "-bounces"))
        {
            i++;
            assert(i < argc);
            bounces = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-weight"))
        {
            i++;
            assert(i < argc);
            weight = atof(argv[i]);
        }
        else if (!strcmp(argv[i], "-grid"))
        {
            i++;
            assert(i < argc);
            nx = atoi(argv[i]);
            i++;
            assert(i < argc);
            ny = atoi(argv[i]);
            i++;
            assert(i < argc);
            nz = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-visualize_grid"))
        {
            visualize_grid = true;
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

    Grid *grid = nullptr;
    if (nx != 0) {
        grid = new Grid(parse.getGroup()->getBoundingBox(), nx, ny, nz);
        parse.getGroup()->insertIntoGrid(grid, nullptr);
    }

    if (gui)
    {
        // 创建一个GLCanvas实例
        GLCanvas canvas;
        glutInit(&argc, argv);
        // 初始化GLCanvas
        canvas.initialize(&parse, renderFunction, traceRayFunction, grid, visualize_grid);
    }
    else
    {
        renderFunction();
    }
}