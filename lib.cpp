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

static inline double deg2rad(double deg) { return deg * M_PI / 180.0; }
static inline double sq(double x) { return x*x; }

static inline double wrapToPi(double x) {
    constexpr double TWO_PI = 2.0 * M_PI;
    double n = (int)(x / TWO_PI);
    x = x - n * TWO_PI;

    if (x > M_PI)
        x -= TWO_PI;
    else if (x < -M_PI)
        x += TWO_PI;

    return x;
}

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

    void removeVertex(std::size_t idx) {
        const std::size_t n = vertices.size();
        if (idx >= n || idx < 0) return;

        if (n <= 3) {
            vertices.erase(vertices.begin() + idx);
            areaAccum = 0.0;
            return;
        }

        double difference = 0.0;
        // Приграничный случай - удаление первой точки
        if(idx == 0) {
            geodesicSegment(vertices[idx], vertices[1], difference);
            areaAccum -= difference;
            vertices.erase(vertices.begin() + idx);
            return;
        }    
        
        // Приграничный случай - удаление последней точки
        if(idx == n - 1) {
            geodesicSegment(vertices[idx- 1], vertices[idx], difference);
            areaAccum -= difference;
            vertices.erase(vertices.begin() + idx);
            return;
        }

        geodesicSegment(vertices[idx - 1], vertices[idx], difference);
        geodesicSegment(vertices[idx - 1], vertices[idx], difference);
        areaAccum -= difference;

        vertices.erase(vertices.begin() + idx);
        geodesicSegment(vertices[idx - 1], vertices[idx + 1], areaAccum);
    }


    double getArea() {
        if (vertices.size() <= 2) {
            return -1.0;
        }
        
        double area = areaAccum;
        geodesicSegment(vertices.back(), vertices.front(), area);
        return std::abs(area);
    }

private:
    std::vector<GeoPoint> vertices;
    double areaAccum = 0;

    void geodesicSegment(const GeoPoint &p1, const GeoPoint &p2, double &S) {
        double lat1 = deg2rad(p1.lat);
        double lon1 = deg2rad(p1.lon);
        double lat2 = deg2rad(p2.lat);
        double lon2 = deg2rad(p2.lon);

        double dlon = wrapToPi(lon2 - lon1);

        S += dlon * (2 + std::sin(lat1) + std::sin(lat2)) * R*R / 2.0;
    }
};

int main() {
    PolygonAreaCalculator poly;
    
    // Кейс с накладной фигурой
    poly.addVertex(61.68354893219501,
              60.56880738002414);  
    poly.addVertex(61.65594533075729,
              60.32366919981277);   
    poly.addVertex(61.84453425177535,
              60.43289695086419);   
    poly.addVertex(61.51191231249857,
              60.40362319099275);
    poly.addVertex(61.38055869617483,
              60.410227936145304);
    poly.addVertex(61.88482701465526,
              60.134935490326114);
    poly.addVertex(62.32963266911992,
              60.30545286024636);

    std::cout << "Площадь до удаления точки: " << static_cast<long long>(poly.getArea()) << " м²\n";

    PolygonAreaCalculator poly2;
    poly2.addVertex(59.72998847659571,
              60.70898299946231);
    poly2.addVertex(58.93878757730823,
              60.4334066633032);
    poly2.addVertex(59.804197257737115,
              60.112490038459356);
    poly2.addVertex(60.44760223858603,
              60.412197598240766);

    std::cout << "Площадь до удаления точки: " << static_cast<long long>(poly2.getArea()) << " м²\n";

    poly2.removeVertex(0);
    std::cout << "Площадь после удаления точки: " << static_cast<long long>(poly2.getArea()) << " м²\n";

    return 0;
}
