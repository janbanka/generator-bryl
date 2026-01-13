#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <QObject>

struct SphereParameters {
    float radius = 0.75f;
    int rings = 32;
    int sectors = 32;
};

struct CylinderParameters {
    float radius = 0.5f;
    float height = 1.0f;
    int sides = 32;
};

struct CuboidParameters {
    float width = 1.0f;
    float height = 1.0f;
    float depth = 1.0f;
};

struct ConeParameters {
    float radius = 0.5f;
    float height = 1.0f;
    int sides = 32;
};

struct TorusParameters {
    float majorRadius = 0.6f;
    float minorRadius = 0.2f;
    int majorSegments = 32;
    int minorSegments = 32;
};

struct CatenoidParameters {
    float c = 0.3f;
    float v_range = 0.5f;
    int u_segments = 32;
    int v_segments = 32;
};

struct HelicoidParameters {
    float radius = 0.5f;
    float rotations = 2.0f;
    float alpha = 0.1f;
    int radius_segments = 16;
    int theta_segments = 64;
};

struct MobiusStripParameters {
    float radius = 1.0f;
    float width = 0.3f;
    int u_segments = 64;
    int v_segments = 16;
};

struct KostkaMengeraParameters {
    int level = 2;
};


#endif // PARAMETERS_H
