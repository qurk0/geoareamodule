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


class PolygonAreaCalculator {
public:
    void addVertex(double lon, double lat) {
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

    void geodesicSegment(const GeoPoint &p1, const GeoPoint &p2, double &S) {
        double lat1 = deg2rad(p1.lat);
        double lon1 = deg2rad(p1.lon);
        double lat2 = deg2rad(p2.lat);
        double lon2 = deg2rad(p2.lon);

        S += (lon2 - lon1) * (2 + std::sin(lat1) + std::sin(lat2)) * R*R / 2.0;
    }
};

int main() {
    PolygonAreaCalculator poly;
    
    poly.addVertex(-126.50178084874047,
              62.55180394517623);  
    poly.addVertex(149.31385506563117,
              62.55180394517623);   
    poly.addVertex(149.31385506563117,
              82.53143984408504);   
    poly.addVertex(-126.50178084874047,
              82.53143984408504);
    // poly.addVertex(49.781084656594174,
    //           60.69909623397433);

    std::cout << "Площадь: " << static_cast<long long>(poly.getArea()) << " м²\n";

    return 0;
}
