#include "camera.h"
#include "glCanvas.h"
#include "image.h"
#include "rayTracer.h"
#include "raytracing_stats.h"
#include "scene_parser.h"
#include <GL/freeglut.h>
#include <cstring>
#include <iostream>

char *input_file = nullptr;
int width = 100;
int height = 100;
char *output_file = nullptr;
float depth_min = 0;
float depth_max = 1;
char *depth_file = nullptr;
char *normal_file = nullptr;
bool shade_back = false;
bool shadows = false;
int max_bounces = 0;
float cutoff_weight = 1.0;
bool gui = false;
int theta_steps = 0;
int phi_steps = 0;
bool gouraud = false;
bool visualize_grid = false;
int nx = 0, ny = 0, nz = 0;
bool stats = false;

void traceRayFunction(float x, float y)
{
    // debug
    // cout << x << "" << y << endl;
    // 创建和解析场景
    SceneParser parse = SceneParser(input_file);
    Ray ray = parse.getCamera()->generateRay(Vec2f(x, y));
    RayTracer tracer(&parse, max_bounces, cutoff_weight);
    float tmin = 0.0001f;
    Hit hit(INFINITY);
    tracer.traceRay(ray, tmin, 0, 1.0, hit);
    Hit hit2(INFINITY);
    tracer.getGrid()->intersect(ray, hit2, tmin);
}

void renderFunction()
{
    // 在这里实现光线追踪代码
    // 创建和解析场景
    SceneParser parse = SceneParser(input_file);
    // 创建相机
    Camera *camera = parse.getCamera();
    // 创建图像缓冲区
    // 由于最后一个测试样例是矩形，所以就不将其变成正方形
    Image image(width, height);
    // 初始化图像颜色
    image.SetAllPixels(parse.getBackgroundColor());

    RayTracer tracer(&parse, max_bounces, cutoff_weight);
    // debug
    // cout << tracer.getGrid()->getBoundingBox()->getMin() << " " << tracer.getGrid()->getBoundingBox()->getMax() << endl;
    if (nx != 0)
        RayTracingStats::Initialize(width, height, tracer.getGrid()->getBoundingBox(), nx, ny, nz);
    else
        RayTracingStats::Initialize(width, height, nullptr, 0, 0, 0);
    for (int y = 0; y < width; y++)
    {
        for (int x = 0; x < height; x++)
        {
            RayTracingStats::IncrementNumNonShadowRays(); // 增加一条非阴影线数量
            // 将像素坐标归一化到 [0, 1]
            Vec2f point(float(y) / width, float(x) / height);
            // 生成射线
            Ray ray = camera->generateRay(point);
            float tmin = 0.0001f;
            Hit hit(INFINITY);
            Vec3f color = tracer.traceRay(ray, tmin, 0, 1.0, hit);
            image.SetPixel(y, x, color);
        }
    }
    if (output_file != nullptr)
    {
        image.SaveTGA(output_file);
    }
    if (stats)
    {
        RayTracingStats::PrintStatistics();
    }
}

int main(int argc, char **argv)
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
        else if (!strcmp(argv[i], "-shadows"))
        {
            shadows = true;
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
        else if (!strcmp(argv[i], "-bounces"))
        {
            i++;
            assert(i < argc);
            max_bounces = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-weight"))
        {
            i++;
            assert(i < argc);
            cutoff_weight = atof(argv[i]);
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
        else if (!strcmp(argv[i], "-stats"))
        {
            stats = true;
        }
        else
        {
            printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
            assert(0);
        }
    }

    // 创建和解析场景
    SceneParser parse(input_file);

    // gui
    if (gui)
    {
        Grid *grid = nullptr;
        if (nx != 0)
        {
            grid = new Grid(parse.getGroup()->getBoundingBox(), nx, ny, nz);
            parse.getGroup()->insertIntoGrid(grid, nullptr);
        }
        glutInit(&argc, argv);
        GLCanvas glCanvas;
        glCanvas.initialize(&parse, renderFunction, traceRayFunction, grid, visualize_grid);
        return 0;
    }
    else
    {
        renderFunction();
    }

    return 0;
}
