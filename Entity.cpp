#pragma once

#include "lab_m1/Tema1/Entity.h"
#include <vector>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <iostream>
#include <string>
#include <list>

using namespace std;


bool Entity::checkCollision(const Circle& circle, const Rectangle& rectangle) {

    glm::vec2 halves(rectangle.width / 2, rectangle.height / 2);

    glm::vec2 rectangleCenter(
        rectangle.corner.x + halves.x,
        rectangle.corner.y + halves.y
    );

    glm::vec2 difference = circle.center - rectangleCenter;
    glm::vec2 clamped = glm::clamp(difference, -halves, halves);

    glm::vec2 closest = rectangleCenter + clamped;

    difference = closest - circle.center;
    return glm::length(difference) < circle.radius;
}
bool Entity::checkCollision(const Rectangle& rectangle1, const Rectangle& rectangle2) {
    // collision x
    bool collisionX = rectangle1.corner.x + rectangle1.width >= rectangle2.corner.x &&
        rectangle2.corner.x + rectangle2.width >= rectangle1.corner.x;
    // collision y
    bool collisionY = rectangle1.corner.y + rectangle1.height >= rectangle2.corner.y &&
        rectangle2.corner.y + rectangle2.height >= rectangle1.corner.y;

    return collisionX && collisionY;
}

bool Entity::checkCollision(const Circle& circle1, const Circle& circle2) {
    return glm::length(circle1.center - circle2.center) <= circle1.radius + circle2.radius;
}