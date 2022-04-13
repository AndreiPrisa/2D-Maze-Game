#pragma once

#include <glm/ext/vector_float2.hpp>
#include <list>


constexpr auto NORMAL = 1;
constexpr auto GUNNER = 2;

constexpr auto FRIENDLY = 3;
constexpr auto HOSTILE = 4;

constexpr auto HEALTH = 1;
constexpr auto FIRERATE = 2;
constexpr auto SHOTGUN = 3;
constexpr auto CANNON = 4;

class Tema1;

namespace Entity
{
    struct Rectangle
    {
        Rectangle() : corner(0), width(1), height(1) {}
        Rectangle(glm::vec2 corner, float width, float height)
            : corner(corner), width(width), height(height) {}
        glm::vec2 corner;
        float width;
        float height;
    };

    struct Circle
    {
        Circle() : center(0), radius(1) {}
        Circle(glm::vec2 centre, float radius)
            : center(centre), radius(radius) {}
        glm::vec2 center;
        float radius;
    };

    class Enemy {
    public:
        Enemy() : body(), scoreValue(0), damage(0), speed(0), health(0), handDistance(0), handSize(1), type(NORMAL) {}
        Enemy(Rectangle body, float scoreValue, float damage, float speed, float health, float handDistance, float handSize, int type)
            : body(body), scoreValue(scoreValue), damage(damage), speed(speed), health(health), handDistance(handDistance), handSize(handSize), type(type) {}
        Rectangle body;
        float scoreValue;
        float damage;
        float speed;
        float health;
        float handDistance;
        float handSize;
        int type;
    };

    class Gunner : public Enemy {
    public:
        Gunner() : fireTime(1), bulletDamage(15), internalTimer(0) {}
        Gunner(Rectangle body, float scoreValue, float damage, float speed, float health, float handDistance, float handSize, int type, float fireTime, float bulletDamage) :
            fireTime(fireTime), bulletDamage(bulletDamage), internalTimer(0) {
            this->body = body;
            this->scoreValue = scoreValue;
            this->damage = damage;
            this->speed = speed;
            this->health = health;
            this->handDistance = handDistance;
            this->handSize = handSize;
            this->type = type;
        }
        float fireTime;
        float internalTimer;
        float bulletDamage;
    };

    struct Projectile {
        Projectile() : body(), trajectory(0), distanceTravelled(0), damage(0), type(FRIENDLY) { }
        Projectile(Rectangle rectangle, glm::vec2 trajectory, float damage, int type)
            : body(rectangle), trajectory(trajectory), distanceTravelled(0), damage(damage), type(type) {}
        Rectangle body;
        glm::vec2 trajectory;
        float distanceTravelled;
        float damage;
        int type;
    };

    struct Pickup {
        Pickup() : body(), timer(0), powerupTimer(0), type(0) {}
        Pickup(Circle body, float powerupTimer, int type)
            : body(body), timer(0), type(type), powerupTimer(powerupTimer) {}
        Circle body;
        float timer;
        float powerupTimer;
        int type;
    };

    bool checkCollision(const Circle& circle1, const Circle& circle2);
    bool checkCollision(const Circle& circle, const Rectangle& rectangle);
    bool checkCollision(const Rectangle& rectangle1, const Rectangle& rectangle2);
    bool checkCollision(Projectile bullet, const Enemy& enemy, float maxDistance);

};