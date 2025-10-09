#include <cmath>
#include <vector>
#include <iostream>

struct GeoPoint {
    double lat; // широта в градусах
    double lon; // долгота в градусах
};

// WGS84 параметры
const double a = 6378137.0;          // большая полуось
const double f = 1/298.257223563;    // сжатие
const double b = a * (1 - f);        // малая полуось
const double e2 = f * (2 - f);       // квадрат эксцентриситета

inline double deg2rad(double deg) { return deg * M_PI / 180.0; }
inline double sq(double x) { return x*x; }

const double R = std::sqrt((sq(a) + sq(b) * ( (e2==0)?1: std::atanh(std::sqrt(e2))/std::sqrt(e2) ))/2.0);

void geodesicSegment(const GeoPoint &p1, const GeoPoint &p2, double &S) {
    double lat1 = deg2rad(p1.lat);
    double lon1 = deg2rad(p1.lon);
    double lat2 = deg2rad(p2.lat);
    double lon2 = deg2rad(p2.lon);

    S += (lon2 - lon1) * (2 + std::sin(lat1) + std::sin(lat2)) * R*R / 2.0;
}

class PolygonAreaEllipsoid {
public:
    void addVertex(double lat, double lon) {
        GeoPoint p{lat, lon};
        vertices.push_back(p);

        if (vertices.size() > 1) {
            geodesicSegment(vertices[vertices.size()-2], p, areaAccum);
        }
    }

    double getArea() {
        if (vertices.size() > 2)
            geodesicSegment(vertices.back(), vertices.front(), areaAccum);
        return std::abs(areaAccum);
    }

private:
    std::vector<GeoPoint> vertices;
    double areaAccum = 0;
};

int main() {
    PolygonAreaEllipsoid poly;

    // poly.addVertex(0.0, 0.0);
    // poly.addVertex(0.001, 0.003);
    // poly.addVertex(0.004, 0.005);
    // poly.addVertex(0.006, 0.002);
    // poly.addVertex(0.005, -0.002);
    // poly.addVertex(0.002, -0.003);

    // poly.addVertex(48.0, 2.0);   // Точка 1: около Парижа, Франция
    // poly.addVertex(52.0, 13.0);  // Точка 2: около Берлина, Германия
    // poly.addVertex(50.0, 20.0);  // Точка 3: около Кракова, Польша
    // poly.addVertex(45.0, 18.0);  // Точка 4: около Загреба, Хорватия
    // poly.addVertex(43.0, 7.0);   // Точка 5: около Генуи, Италия

    // poly.addVertex(0.01, 0.0);
    // poly.addVertex(0.009, 0.002);
    // poly.addVertex(0.008, 0.004);
    // poly.addVertex(0.007, 0.006);
    // poly.addVertex(0.006, 0.008);
    // poly.addVertex(0.005, 0.01);
    // poly.addVertex(0.004, 0.008);
    // poly.addVertex(0.003, 0.006);
    // poly.addVertex(0.002, 0.004);
    // poly.addVertex(0.001, 0.002);
    // poly.addVertex(0.0, 0.0);
    // poly.addVertex(0.001, -0.002);
    // poly.addVertex(0.002, -0.004);
    // poly.addVertex(0.003, -0.006);
    // poly.addVertex(0.004, -0.008);
    // poly.addVertex(0.005, -0.01);
    // poly.addVertex(0.006, -0.008);
    // poly.addVertex(0.007, -0.006);
    // poly.addVertex(0.008, -0.004);
    // poly.addVertex(0.009, -0.002);
    // poly.addVertex(0.01, 0.0);
    
    poly.addVertex(65.0, -10.0);  // Точка 1: около Исландии (Северная Атлантика)
poly.addVertex(60.0, 30.0);   // Точка 2: около Санкт-Петербурга, Россия
poly.addVertex(50.0, 80.0);   // Точка 3: около Новосибирска, Россия
poly.addVertex(35.0, 120.0);  // Точка 4: около Шанхая, Китай
poly.addVertex(45.0, 10.0);   // Точка 5: около Милана, Италия

    std::cout << "Площадь: " << static_cast<long long>(poly.getArea()) << " м²\n";

    return 0;
}
