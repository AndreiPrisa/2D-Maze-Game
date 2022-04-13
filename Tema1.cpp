#include <vector>
#include <iostream>

#include "lab_m1/Tema1/Tema1.h"
#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/lab3/object2D.h"
#include "lab_m1/Tema1/Entity.h"

using namespace std;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    flag = false;
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    // hide cursor
    window->HidePointer();

    glm::vec3 centre = glm::vec3(0.1f, 0.1f, 0);
    glm::vec3 corner = glm::vec3(0);

    float squareSide = 1;

    logicSpace.x = 0;       // logic x
    logicSpace.y = 0;       // logic y
    logicSpace.width = 16 * 1.1f;   // logic width
    logicSpace.height = 9 * 1.1f;  // logic height

    // player vision border
    visionBorder = Rectangle(glm::vec2(logicSpace.x, logicSpace.y), logicSpace.width, logicSpace.height);

    // for cursor detection
    UpdateDiff();

    // HUD
    float barLength, barHeight;
    barLength = 5;
    barHeight = 1;
    barDistance = 0.5f;
    maxScoreBar = Rectangle(glm::vec2(barDistance, logicSpace.height - barDistance - barHeight), barLength, barHeight);
    maxHealthBar = Rectangle(glm::vec2(logicSpace.width - barDistance - barLength, logicSpace.height - barDistance - barHeight), barLength, barHeight);
    maxPowerBar = Rectangle(glm::vec2(0, 0), barLength, barHeight / 2);
    score = 0;
    maxScore = 2000;
    unit = 1;

    // positioning player in the middle of the screen
    glm::vec2 playerPos = glm::vec2(logicSpace.x + logicSpace.width / 2, logicSpace.y + logicSpace.height / 2);

    // map margins
    map = Map(-25, -15, 70, 40);

    mapLogicSpace.x = map.x;       // logic x
    mapLogicSpace.y = map.y;       // logic y
    mapLogicSpace.width = map.width;   // logic width
    mapLogicSpace.height = map.height;  // logic height

    // player
    maxHealth = 100;
    player = Player(playerPos, maxHealth);
    bulletSpawnDistance = player.radius + 0.4f;
    fireRate = 0.5f;
    fireRateTimer = fireRate;
    timer = 0;
    playerBulletDamage = 10;
    playerSpeed = 7.5f;

    mainBody = Entity::Circle(player.pos, player.radius);
    mousePos.x = logicSpace.x + logicSpace.width / 2 + player.radius;
    mousePos.y = logicSpace.y + logicSpace.height / 2;

    Mesh* playerHead = object2D::CreatePolygon("playerHead", 
        centre,
        player.radius, glm::vec3(1, 0.85f, 0.40f), 5, true);
    AddMeshToList(playerHead);

    Mesh* playerHand = object2D::CreatePolygon("playerHand",
        centre,
        player.handRadius, glm::vec3(0.74f, 0.56f, 0.5), 5, true);
    AddMeshToList(playerHand);

    playerStats = Stats();
    /////

    // enemies

    glm::vec2 normalSize = glm::vec2(1.3f, 1.3f);
    float handDistance = 1.2f;
    float handSize = 0.5f;
 
    spawnTimer = 0;
    MaxTimeSpawn = 10;
    maxSpeed = 6;
    noOfSpawns = 5;
    
    float damage = 10, health = 10, scoreValue = 10;
    normalEnemy = Enemy(Rectangle(corner, normalSize.x, normalSize.y), scoreValue, damage, 0, health, handDistance, handSize, NORMAL);

    damage = 20; health = 20; scoreValue = 20;
    float bulletDamage = 10, fireRateGunner = 1;
    float scaling = 1.3f;
    normalGunner = Gunner(Rectangle(corner, normalSize.x * scaling, normalSize.y * scaling), scoreValue, damage, 0, health, handDistance * scaling, handSize * scaling, GUNNER, fireRateGunner, bulletDamage);
    
    Mesh* enemy = object2D::CreateSquare("enemy", corner, squareSide, glm::vec3(0.7, 0, 0), true);
    AddMeshToList(enemy);

    Mesh* gunner = object2D::CreateSquare("gunner", corner, squareSide, glm::vec3(0, 0.5, 0), true);
    AddMeshToList(gunner);
    ////

    // obstacles
    Mesh* obstacle = object2D::CreateSquare("obstacle", corner, squareSide, glm::vec3(0, 1, 0), true);
    AddMeshToList(obstacle);
    obstacles = {
        Entity::Rectangle(glm::vec2(map.x + 1, map.y + 1), 5, 15),
        Entity::Rectangle(glm::vec2(map.x + map.width / 2, map.y + map.height / 10), 10, 5),
        Entity::Rectangle(glm::vec2(map.x + map.width / 1.5, map.y +  map.height / 2), 5, 5),
        Entity::Rectangle(glm::vec2(map.x + 50, map.y + 10), 6, 3),
        Entity::Rectangle(glm::vec2(map.x + 30, map.y + 30), 3, 5),
        Entity::Rectangle(glm::vec2(map.x + 20, map.y + 20), 2, 3),
        Entity::Rectangle(glm::vec2(map.x + 45, map.y + 30), 2, 7),
        Entity::Rectangle(glm::vec2(map.x + 25, map.y + 33), 3, 3),
        Entity::Rectangle(glm::vec2(map.x + 5, map.y + 35), 6, 2),
        Entity::Rectangle(glm::vec2(map.x + 24, map.y + 15), 5, 5),
        Entity::Rectangle(glm::vec2(map.x + 21, map.y + 5), 3, 7),
        Entity::Rectangle(glm::vec2(map.x + 4, map.y + 20), 6, 6),
        Entity::Rectangle(glm::vec2(map.x + 4, map.y + 27), 5, 7),
        Entity::Rectangle(glm::vec2(map.x + 4, map.y + 20), 2, 2),
        Entity::Rectangle(glm::vec2(map.x + 65, map.y + 5), 2, 5),
        Entity::Rectangle(glm::vec2(map.x + 55, map.y + 14), 5, 5),
        Entity::Rectangle(glm::vec2(map.x + 60, map.y + 30), 2, 5),
        Entity::Rectangle(glm::vec2(map.x + 55, map.y + 25), 6, 4),
        Entity::Rectangle(glm::vec2(map.x + 55, map.y + 14), 5, 5),
        Entity::Rectangle(glm::vec2(map.x + 37, map.y + 25), 4, 8),
        Entity::Rectangle(glm::vec2(map.x + 10, map.y + 3), 8, 8),
        Entity::Rectangle(glm::vec2(map.x + 50, map.y + 3), 3, 8),
        Entity::Rectangle(glm::vec2(map.x + 35, map.y), 2, 2),
        Entity::Rectangle(glm::vec2(map.x + 50, map.y + 35), 3, 5),
        Entity::Rectangle(glm::vec2(map.x + 38, map.y + 18), 2, 2),
        Entity::Rectangle(glm::vec2(map.x + 15, map.y + 30), 9, 4),
        Entity::Rectangle(glm::vec2(map.x + 35, map.y), 2, 2),
        Entity::Rectangle(glm::vec2(map.x + 65, map.y + 15), 3, 3),
        Entity::Rectangle(glm::vec2(map.x + 28, map.y + 5), 7, 9),
        Entity::Rectangle(glm::vec2(map.x + 15, map.y + 15), 5, 2),
        Entity::Rectangle(glm::vec2(map.x + 60, map.y + 2), 4, 8),
        Entity::Rectangle(glm::vec2(map.x + 39, map.y + 36), 6, 3),
        Entity::Rectangle(glm::vec2(map.x + 64, map.y + 30), 4, 6),
    };
    ////

    // projectiles
    Mesh* projectileMesh = object2D::CreateSquare("projectileMesh", corner, squareSide, glm::vec3(0, 0, 0), true);
    projectileSize = glm::vec2(0.3f, 0.3f);
    projectileSpeed = 15;
    maxDistance = 10;
    AddMeshToList(projectileMesh);
    ////

    // World barrier
    Mesh* barrier = object2D::CreateSquare("barrier", corner, 1, glm::vec3(0.5f, 0.5f, 0.5f), true);
    AddMeshToList(barrier);
    
    // player cursor
    Mesh* cursor = object2D::CreatePolygon("cursor", centre, 1, glm::vec3(0, 0, 1), 120, true);
    AddMeshToList(cursor);
    // cursor current rotation
    angularStep = 0;


    // primitive black border
    thickness = 0.30f;
    Mesh* squareBorder = object2D::CreateSquare("squareBorder", corner, unit, glm::vec3(0, 0, 0), true);
    AddMeshToList(squareBorder);

    Mesh* circleBorder = object2D::CreatePolygon("circleBorder", centre, unit, glm::vec3(0, 0, 0), 5, true);
    AddMeshToList(circleBorder);

    // HUD
    Mesh* healthBarBorder = object2D::CreateSquare("healthBorder", corner, squareSide, glm::vec3(0.7, 0, 0));
    AddMeshToList(healthBarBorder);

    Mesh* healthBarFull = object2D::CreateSquare("healthFull", corner, squareSide, glm::vec3(0.7, 0, 0), true);
    AddMeshToList(healthBarFull);

    Mesh* scoreBarBorder = object2D::CreateSquare("scoreBorder", corner, squareSide, glm::vec3(1, 1, 0));
    AddMeshToList(scoreBarBorder);

    Mesh* scoreBarFull = object2D::CreateSquare("scoreFull", corner, squareSide, glm::vec3(1, 1, 0), true);
    AddMeshToList(scoreBarFull);

    Mesh* powerBorder = object2D::CreateSquare("powerBorder", corner, squareSide, glm::vec3(0.5, 0, 0.5));
    AddMeshToList(powerBorder);

    Mesh* powerFull = object2D::CreateSquare("powerFull", corner, squareSide, glm::vec3(0.5, 0, 0.5), true);
    AddMeshToList(powerFull);

    // PICKUP
    activeTime = 5;
    healthRegen = 30;
    pickupStandardRadius = 0.35f;
    pickupTimer = 0;
    Mesh* greenCircle = object2D::CreatePolygon("greenCircle", centre, unit, glm::vec3(0, 1, 0), 5, true);
    AddMeshToList(greenCircle);
    Mesh* whiteCircle = object2D::CreatePolygon("whiteCircle", centre, unit, glm::vec3(1, 1, 1), 5, true);
    AddMeshToList(whiteCircle);
    Mesh* redCircle = object2D::CreatePolygon("redCircle", centre, unit, glm::vec3(0.9f, 0, 0), 5, true);
    AddMeshToList(redCircle);
    Mesh* whiteSquare = object2D::CreateSquare("whiteSquare", corner - glm::vec3(squareSide / 2, squareSide / 2, 0), squareSide, glm::vec3(1, 1, 1), true);
    AddMeshToList(whiteSquare);
    Mesh* blackSquare = object2D::CreateSquare("blackSquare", corner - glm::vec3(squareSide / 2, squareSide / 2, 0), squareSide, glm::vec3(0, 0, 0), true);
    AddMeshToList(blackSquare);
    Mesh* yellowSquare = object2D::CreateSquare("yellowSquare", corner - glm::vec3(squareSide / 2, squareSide / 2, 0), squareSide, glm::vec3(1, 1, 0), true);
    AddMeshToList(yellowSquare);
}

void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    if (clear) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}

// Uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}

void Tema1::UpdateDiff() {
    difference.x = 0; // width
    difference.y = 0; // height
    glm::ivec2 resolution = window->GetResolution();
    float visResolution = (float) resolution.x / resolution.y;
    float logicResolution = logicSpace.width / logicSpace.height;

    if (visResolution > logicResolution) {
        difference.x = visResolution * logicSpace.height - logicSpace.width;
    }
    else if ((visResolution < logicResolution)) {
        difference.y = 1 / visResolution  * logicSpace.width - logicSpace.height;
    }
}

bool Tema1::canMove() {
    bool flag = true;

    std::vector<Rectangle>::iterator i = obstacles.begin();
    while (i != obstacles.end()) {
        if (checkCollision(mainBody, *i)) {
            flag = false;
            break;
        }
        i++;
    }

    return flag;
}

void Tema1::pickupGenerator() {


    int size = 4;
    int possibleTypes[] = { HEALTH, FIRERATE, SHOTGUN, CANNON };
    int maxTime[] = { 0, 10, 10, 10};
    int frequency[] = { 25, 25, 25, 25};

    Pickup newPickup = Pickup(Circle(glm::vec2(0, 0), pickupStandardRadius), 0, 0);
    bool flag;
    do {
        float low = visionBorder.corner.x + pickupStandardRadius;
        float high = visionBorder.corner.x + visionBorder.width - pickupStandardRadius;
        newPickup.body.center.x = low + rand() /  (RAND_MAX / (high - low));

        low = visionBorder.corner.y + pickupStandardRadius;
        high = visionBorder.corner.y + visionBorder.height - pickupStandardRadius;
        newPickup.body.center.y = low + rand() / (RAND_MAX / (high - low));
        flag = false;

        for (Rectangle obj : obstacles) {
            if (checkCollision(newPickup.body, obj)) {
                flag = true;
                break;
            }
        }

        if (outOfMap(newPickup.body) || checkCollision(newPickup.body, mainBody)) {
            flag = true;
        }

    } while (flag);
    int val = 1 + rand() /  (RAND_MAX / 99);
    int count = 0;

    for (int i = 0; i < size; i++) {
        if (val <= count + frequency[i]) {
            newPickup.type = possibleTypes[i];
            newPickup.powerupTimer = maxTime[i];
            break;
        }
        count += frequency[i];
    }
    newPickup.timer = activeTime;
    spawnedPickups.push_back(newPickup);

}

void Tema1::enemySpawner(float maxSpeed, float minSpeed)
{
    float gunnerChance = 20;

    for (int i = 0; i < noOfSpawns; i++) {
        Enemy *enemy = new Enemy();
        float rez = 1 + rand() / (RAND_MAX / 99);
        if (rez <= gunnerChance) {
            Gunner* gunner = new Gunner();
            gunner->body = normalGunner.body;
            gunner->damage = normalGunner.damage;
            gunner->handDistance = normalGunner.handDistance;
            gunner->handSize = normalGunner.handSize;
            gunner->health = normalGunner.health;
            gunner->scoreValue = normalGunner.scoreValue;
            gunner->speed = normalGunner.speed;
            gunner->type = normalGunner.type;
            enemy = gunner;
        }
        else {
            enemy->body = normalEnemy.body;
            enemy->damage = normalEnemy.damage;
            enemy->handDistance = normalEnemy.handDistance;
            enemy->handSize = normalEnemy.handSize;
            enemy->health = normalEnemy.health;
            enemy->scoreValue = normalEnemy.scoreValue;
            enemy->speed = normalEnemy.speed;
            enemy->type = normalEnemy.type;
        }

        int speed = std::min(2.5f * playerSpeed, minSpeed + rand() / (RAND_MAX / (maxSpeed - minSpeed)));
        
        enemy->speed = speed;
        float low, high;

        do {
            low = map.x;
            high = low + map.width - enemy->body.width;
            enemy->body.corner.x = low + rand() /  (RAND_MAX / (high - low));

            low = map.y;
            high = low + map.height - enemy->body.height;
            enemy->body.corner.y = low + rand() / (RAND_MAX / (high - low));
        } while (checkCollision(visionBorder, enemy->body));

        enemies.push_back(enemy);
    }
}

bool Tema1::outOfMap(Rectangle body) {
    bool leftSide = body.corner.x <= map.x;
    bool downSide = body.corner.y <= map.y;
    bool rightSide = body.corner.x + body.width >= map.x + map.width;
    bool upSide = body.corner.y + body.height >= map.y + map.height;
    return leftSide || downSide || rightSide || upSide;
}

bool Tema1::outOfMap(Circle body) {
    bool leftSide = body.center.x - body.radius <= map.x;
    bool downSide = body.center.y - body.radius  <= map.y;
    bool rightSide = body.center.x + body.radius >= map.x + map.width;
    bool upSide = body.center.y + body.radius >= map.y + map.height;
    return leftSide || downSide || rightSide || upSide;
}

void Tema1::increaseDifficulty() {
    int spawnCap = 10;
    if (timer >= 20) {
        maxSpeed += 0.5f;
        normalEnemy.damage += 1;
        noOfSpawns = min(noOfSpawns + 1, spawnCap);
        timer = 0;
    }
}

void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::PickupRenderer(glm::mat3 vm, glm::vec2 translate, glm::vec2 scale, int type) {
    glm::mat3 modelMatrix;
    string circleType = "NaN";
    switch (type) {
    case HEALTH:

        // cross horizontal
        modelMatrix = vm * transform2D::Translate(translate.x, translate.y);
        modelMatrix *= transform2D::Scale(1.5f * unit, unit / 2);
        RenderMesh2D(meshes["whiteSquare"], shaders["VertexColor"], modelMatrix * transform2D::Scale(scale.x, scale.y));

        // cross vertical
        modelMatrix = vm * transform2D::Translate(translate.x, translate.y);
        modelMatrix *= transform2D::Scale(unit / 2, 1.5f * unit);
        RenderMesh2D(meshes["whiteSquare"], shaders["VertexColor"], modelMatrix * transform2D::Scale(scale.x, scale.y));

        // green background
        circleType ="greenCircle";

        break;
    case SHOTGUN:

        glm::mat3 bulletScale = transform2D::Scale(0.45f * unit, 0.45f * unit) * transform2D::Scale(scale.x, scale.y);
        // bullet left
        modelMatrix = vm * transform2D::Translate(translate.x, translate.y);

        modelMatrix *= transform2D::Translate(-0.1f * unit, -0.1f * unit);
        modelMatrix *= transform2D::Rotate(glm::radians(-45.f));
        RenderMesh2D(meshes["blackSquare"], shaders["VertexColor"], modelMatrix * bulletScale);

        // bullet right
        modelMatrix = vm * transform2D::Translate(translate.x, translate.y);

        modelMatrix = modelMatrix * transform2D::Translate(0.1f * unit, -0.1f * unit);
        modelMatrix *= transform2D::Rotate(glm::radians(45.f));
        RenderMesh2D(meshes["blackSquare"], shaders["VertexColor"], modelMatrix * bulletScale);

        // bullet up
        modelMatrix = vm * transform2D::Translate(translate.x, translate.y);
        modelMatrix = modelMatrix * transform2D::Translate(0, 0.1f * unit);
        RenderMesh2D(meshes["blackSquare"], shaders["VertexColor"], modelMatrix * bulletScale);

        // red background
        circleType = "redCircle";
        break;

    case CANNON:

        glm::mat3 bulletScaleCannon = transform2D::Scale(unit, unit) * transform2D::Scale(scale.x, scale.y);
        // bullet left
        modelMatrix = vm * transform2D::Translate(translate.x, translate.y);
        RenderMesh2D(meshes["blackSquare"], shaders["VertexColor"], modelMatrix * bulletScaleCannon);

        // red background
        circleType = "redCircle";
        break;
    case FIRERATE:

        // lightning pattern
        glm::mat3 scaleMatrix = transform2D::Scale(1.1f * unit, 0.3f * unit) * transform2D::Scale(scale.x, scale.y);
        modelMatrix = vm * transform2D::Translate(translate.x, translate.y);


        // middle
        RenderMesh2D(meshes["yellowSquare"], shaders["VertexColor"], modelMatrix * transform2D::Scale(0.9f, 1) * scaleMatrix);

        // up
        modelMatrix = vm * transform2D::Translate(translate.x, translate.y);
        modelMatrix = modelMatrix * transform2D::Translate(0, 0.1f * unit);
        modelMatrix *= transform2D::Rotate(glm::radians(-45.f));
        RenderMesh2D(meshes["yellowSquare"], shaders["VertexColor"], modelMatrix * scaleMatrix);

        // down
        modelMatrix = vm * transform2D::Translate(translate.x, translate.y);
        modelMatrix = modelMatrix * transform2D::Translate( 0, -0.1f * unit);
        modelMatrix *= transform2D::Rotate(glm::radians(-45.f));
        RenderMesh2D(meshes["yellowSquare"], shaders["VertexColor"], modelMatrix * scaleMatrix);

        // red background
        circleType = "redCircle";
        break;
    }
    // circle draw
    modelMatrix = vm * transform2D::Translate(translate.x, translate.y);
    RenderMesh2D(meshes[circleType], shaders["VertexColor"], modelMatrix * transform2D::Scale(scale.x, scale.y));

    // border
    modelMatrix *= transform2D::Scale(unit + thickness, unit + thickness);
    RenderMesh2D(meshes["circleBorder"], shaders["VertexColor"], modelMatrix * transform2D::Scale(scale.x, scale.y));
}

void Tema1::DrawScene(glm::mat3 vm, float deltaTimeSeconds, bool minimap) {

    float radians = glm::acos(glm::dot(glm::normalize(player.currentLookDir), player.defaultLookDir));
    mousePos.y < player.pos.y ? radians *= -1 : radians *= 1;
    glm::vec2 resolution = window->GetResolution();


    // HUD
    if (!minimap) {
        // health
        // border
        modelMatrix = vm;
        modelMatrix *= transform2D::Translate(logicSpace.x + maxHealthBar.corner.x + difference.x / 2, logicSpace.y + maxHealthBar.corner.y + difference.y / 2);
        modelMatrix *= transform2D::Scale(maxHealthBar.width , maxHealthBar.height);
        RenderMesh2D(meshes["healthBorder"], shaders["VertexColor"], modelMatrix);

        // fill
        modelMatrix = vm;
        modelMatrix *= transform2D::Translate(logicSpace.x + maxHealthBar.corner.x + difference.x / 2, logicSpace.y + maxHealthBar.corner.y + difference.y / 2);
        modelMatrix *= transform2D::Scale((player.health / maxHealth) * maxHealthBar.width, maxHealthBar.height);
        RenderMesh2D(meshes["healthFull"], shaders["VertexColor"], modelMatrix);
        ////
       
        // score
        // border
        modelMatrix = vm;
        modelMatrix *= transform2D::Translate(logicSpace.x + maxScoreBar.corner.x - difference.x / 2, logicSpace.y + maxScoreBar.corner.y + difference.y / 2);
        modelMatrix *= transform2D::Scale(maxScoreBar.width, maxScoreBar.height);
        RenderMesh2D(meshes["scoreBorder"], shaders["VertexColor"], modelMatrix);

        // fill
        modelMatrix = vm;
        modelMatrix *= transform2D::Translate(logicSpace.x + maxScoreBar.corner.x - difference.x / 2, logicSpace.y + maxScoreBar.corner.y + difference.y / 2);
        modelMatrix *= transform2D::Scale((score / maxScore) * maxScoreBar.width, maxScoreBar.height);
        RenderMesh2D(meshes["scoreFull"], shaders["VertexColor"], modelMatrix);
        std::list<Entity::Pickup>::iterator k = activePickups.begin();

        // stats
        std::list<Pickup>::iterator t = activePickups.begin();
        int order = 0;
        while (t != activePickups.end()) {
            t->timer -= deltaTimeSeconds;
            if (t->timer <= 0) {
                switch (t->type) {
                case CANNON:
                    playerStats.cannon = false;
                    break;
                case FIRERATE:
                    playerStats.fireRate = false;
                    break;
                case SHOTGUN:
                    playerStats.shotgun = false;
                    break;
                }
                activePickups.erase(t++);
                continue;
            }

            float OYhealthScoreSpace = max(maxScoreBar.height, maxHealthBar.height) + barDistance;
            float x, y;
            glm::vec2 pickupRenderPos;

            int side = order % 2 == 0 ? -1 : 1;
            x = logicSpace.x + ((order + 1) % 2) * barDistance + (order % 2) * (logicSpace.width - maxPowerBar.width - barDistance) + difference.x / 2 * side;
            y = logicSpace.y + logicSpace.height - OYhealthScoreSpace - (order / 2 + 1) * (maxPowerBar.height + barDistance) + difference.y / 2;

            modelMatrix = vm;
            modelMatrix *= transform2D::Translate(x, y);
            glm::mat3 powerUpMatrix = modelMatrix;
            modelMatrix *= transform2D::Scale(maxPowerBar.width, maxPowerBar.height);
            RenderMesh2D(meshes["powerBorder"], shaders["VertexColor"], modelMatrix);

            modelMatrix = powerUpMatrix;
            modelMatrix *= transform2D::Scale((t->timer / t->powerupTimer) * maxPowerBar.width, maxPowerBar.height);
            RenderMesh2D(meshes["powerFull"], shaders["VertexColor"], modelMatrix);

            pickupRenderPos.x = (order % 2) == 0 ? x + maxPowerBar.width + barDistance / 2 : x - barDistance / 2;
            pickupRenderPos.y = y + pickupStandardRadius;
            PickupRenderer(vm, pickupRenderPos, glm::vec2(maxPowerBar.height / 2, maxPowerBar.height / 2), t->type);
            t++;
            order++;
        }

    }
    // head matrix
    modelMatrix = vm;
    modelMatrix *= transform2D::Translate(player.pos.x, player.pos.y);
    modelMatrix *= transform2D::Rotate(radians);
    glm::mat3 playerHeadMatrix = modelMatrix;

    if (!minimap) {
        // left hand
        modelMatrix = playerHeadMatrix;
        modelMatrix *= transform2D::Translate(player.radius + player.handDistance.x, player.handDistance.y);
        RenderMesh2D(meshes["playerHand"], shaders["VertexColor"], modelMatrix);
        // thickness
        modelMatrix *= transform2D::Scale(player.handRadius + thickness / 2, player.handRadius + thickness / 2);
        RenderMesh2D(meshes["circleBorder"], shaders["VertexColor"], modelMatrix);

        // right hand
        modelMatrix = playerHeadMatrix;
        modelMatrix *= transform2D::Translate(player.radius + player.handDistance.x, -player.handDistance.y);
        RenderMesh2D(meshes["playerHand"], shaders["VertexColor"], modelMatrix);
        // thickness
        modelMatrix *= transform2D::Scale(player.handRadius + thickness / 2, player.handRadius + thickness / 2);
        RenderMesh2D(meshes["circleBorder"], shaders["VertexColor"], modelMatrix);
    }
    // head
    modelMatrix = playerHeadMatrix;
    RenderMesh2D(meshes["playerHead"], shaders["VertexColor"], modelMatrix);

    // thickness
    modelMatrix *= transform2D::Scale(player.radius + thickness / 2, player.radius + thickness / 2);
    RenderMesh2D(meshes["circleBorder"], shaders["VertexColor"], modelMatrix);


    // projectiles
    if (!minimap) {
        std::list<Projectile>::iterator i = projectiles.begin();
        while (i !=projectiles.end()) {
            bool flag = true;
            glm::vec2 movement = i->trajectory * projectileSpeed * deltaTimeSeconds;

            if (glm::length(movement) + i->distanceTravelled >= maxDistance || outOfMap(i->body)) {
                projectiles.erase(i++);
                flag = false;
                continue;
            }

            std::vector<Rectangle>::iterator j = obstacles.begin();
            while (j != obstacles.end()) {
                if (checkCollision(*j, i->body)) {
                    flag = false;
                    projectiles.erase(i++);
                    break;
                }
                j++;
            }
            if (flag) {
                if (i->type == FRIENDLY) {
                    std::list<Enemy*>::iterator j = enemies.begin();
                    while (j != enemies.end()) {
                        if (checkCollision((*j)->body, i->body)) {
                            (*j)->health -= i->damage;
                            if ((*j)->health <= 0) {

                                if (score + (*j)->scoreValue < maxScore) {
                                    score += (*j)->scoreValue;
                                }
                                else {
                                    score = maxScore;
                                }
                                enemies.erase(j++);
                                cout << "SCORE: " << score << endl;
                            }
                            flag = false;
                            projectiles.erase(i++);
                            break;
                        }
                        j++;
                    }
                }
                else {
                    if (checkCollision(mainBody, i->body)) {
                        player.health -= i->damage;
                        flag = false;
                        projectiles.erase(i++);
                    }
                }
            }
            if (flag) {
                float projRot = glm::acos(glm::dot(i->trajectory, player.defaultLookDir));
                i->trajectory.y < 0 ? projRot *= -1 : projRot *= 1;
                modelMatrix = vm;
                modelMatrix *= transform2D::Translate(i->body.corner.x, i->body.corner.y);
                i->body.corner += movement;
                i->distanceTravelled += glm::length(movement);
                modelMatrix *= transform2D::Translate(i->body.width / 2, i->body.height / 2);
                modelMatrix *= transform2D::Rotate(projRot);
                modelMatrix *= transform2D::Translate(-i->body.width / 2, -i->body.height / 2);
                modelMatrix *= transform2D::Scale(i->body.width, i->body.height);
                RenderMesh2D(meshes["projectileMesh"], shaders["VertexColor"], modelMatrix);
                i++;
            }
        }
    }

    // enemies
    std::list<Enemy*>::iterator i = enemies.begin();
    while (i != enemies.end()) {
        if (!minimap) {
            if (checkCollision(mainBody, (*i)->body)) {
                player.health -= (*i)->damage;
                enemies.erase(i++);
                continue;
            }
        }

        // check possible collision
        std::list<Projectile>::iterator k = projectiles.begin();
        Projectile closestBullet;
        bool foundProjectile = false;
        float closestDistance = 0;
        while (k != projectiles.end()) {
            float length = glm::length(k->body.corner - (*i)->body.corner);
            if (k->type == FRIENDLY && glm::length(k->body.corner + 0.1f * k->trajectory - (*i)->body.corner) < length && length < (maxDistance - k->distanceTravelled)) {
                if (!foundProjectile) {
                    foundProjectile = true;
                    closestDistance = length;
                    closestBullet = *k;

                }

                if (length < closestDistance) {
                    closestBullet = *k;
                    closestDistance = length;
                }
            }
            k++;
        }


        glm::vec2 enemyCenter = glm::vec2((*i)->body.corner.x + (*i)->body.width / 2, (*i)->body.corner.y + (*i)->body.height / 2);
        glm::vec2 direction = glm::normalize(player.pos - enemyCenter);

        float enemyRot = glm::acos(glm::dot(direction, player.defaultLookDir));
        direction.y < 0 ? enemyRot *= -1 : enemyRot *= 1;

        glm::vec2 lookDirection = direction;
        if (foundProjectile) {
            direction = glm::vec2(closestBullet.trajectory.y, -closestBullet.trajectory.x);
            if (closestDistance > glm::length((*i)->body.corner + 0.1f * direction - closestBullet.body.corner)) {
                direction *= -1;
            }

        }

        // head matrix
        modelMatrix = vm;
        modelMatrix *= transform2D::Translate((*i)->body.corner.x, (*i)->body.corner.y);
        if (!minimap) {
            modelMatrix *= transform2D::Translate((*i)->body.width / 2, (*i)->body.height / 2);
            modelMatrix *= transform2D::Rotate(enemyRot);
            modelMatrix *= transform2D::Translate(-(*i)->body.width / 2, -(*i)->body.height / 2);
        }
        glm::mat3 enemyHeadMatrix = modelMatrix;
        string enemyString;

        if ((*i)->type == NORMAL) {
            enemyString = "enemy";
        }
        else {
            enemyString = "gunner";
        }

        if (!minimap) {
            // left hand
            modelMatrix = enemyHeadMatrix;
            modelMatrix *= transform2D::Translate((*i)->body.width, (*i)->body.height / 2 + (*i)->handDistance / 2 - (*i)->handSize / 2);
            glm::mat3 leftHandEnemyMatrix = modelMatrix;
            modelMatrix *= transform2D::Scale((*i)->handSize, (*i)->handSize);
            RenderMesh2D(meshes[enemyString], shaders["VertexColor"], modelMatrix);

            // thickness
            modelMatrix = leftHandEnemyMatrix;
            modelMatrix *= transform2D::Translate(-thickness / 2, -thickness / 2);
            modelMatrix *= transform2D::Scale((*i)->handSize + thickness, (*i)->handSize + thickness);
            RenderMesh2D(meshes["squareBorder"], shaders["VertexColor"], modelMatrix);

            // right hand
            modelMatrix = enemyHeadMatrix;
            modelMatrix *= transform2D::Translate((*i)->body.width, (*i)->body.height / 2 - (*i)->handDistance / 2 - (*i)->handSize / 2);
            glm::mat3 rightHandEnemyMatrix = modelMatrix;
            modelMatrix *= transform2D::Scale((*i)->handSize, (*i)->handSize);
            RenderMesh2D(meshes[enemyString], shaders["VertexColor"], modelMatrix);

            // thickness
            modelMatrix = rightHandEnemyMatrix;
            modelMatrix *= transform2D::Translate(-thickness / 2, -thickness / 2);
            modelMatrix *= transform2D::Scale((*i)->handSize + thickness, (*i)->handSize + thickness);
            RenderMesh2D(meshes["squareBorder"], shaders["VertexColor"], modelMatrix);
        }

        // head
        modelMatrix = enemyHeadMatrix;
        modelMatrix *= transform2D::Scale((*i)->body.width, (*i)->body.height);
        RenderMesh2D(meshes[enemyString], shaders["VertexColor"], modelMatrix);

        // thickness
        modelMatrix = enemyHeadMatrix;
        modelMatrix *= transform2D::Translate(- thickness / 2, - thickness / 2);
        modelMatrix *= transform2D::Scale((*i)->body.width + thickness, (*i)->body.height + thickness);
        RenderMesh2D(meshes["squareBorder"], shaders["VertexColor"], modelMatrix);

        if (!minimap) {
            Rectangle nextPos = Rectangle((*i)->body.corner + direction * (*i)->speed * deltaTimeSeconds, (*i)->body.width, (*i)->body.height);
            if (!outOfMap(nextPos)) {
                (*i)->body.corner += direction * (*i)->speed * deltaTimeSeconds;
            }

            if ((*i)->type == GUNNER) {
                Gunner *gunner = static_cast<Gunner*> (&**i);
                gunner->internalTimer += deltaTimeSeconds;
                if (gunner->internalTimer >= gunner->fireTime) {
                    gunner->internalTimer = 0;
                    glm::vec2 spawnPos = gunner->body.corner + glm::vec2(gunner->body.width / 2, gunner->body.height / 2) + 1.2f * gunner->body.width / 2 * lookDirection;
                    Projectile newProjectile = Projectile(Rectangle(spawnPos, projectileSize.x, projectileSize.y), lookDirection, HOSTILE, gunner->bulletDamage);
                    projectiles.push_back(newProjectile);
                }
            }
        }
        i++;
    }

    // on ground pickups
    std::list<Entity::Pickup>::iterator k = spawnedPickups.begin();
    while (k != spawnedPickups.end()) {
        if (!minimap) {
            k->timer -= deltaTimeSeconds;
            if (k->timer <= 0) {
                spawnedPickups.erase(k++);
                continue;
            }

            if (checkCollision(mainBody, k->body)) {
                k->timer = k->powerupTimer;
                switch (k->type) {
                case CANNON:
                    playerStats.cannon = true;
                    break;
                case FIRERATE:
                    playerStats.fireRate = true;
                    break;
                case SHOTGUN:
                    playerStats.shotgun = true;
                    break;
                case HEALTH:
                    player.health = player.health + healthRegen > maxHealth ? maxHealth : player.health + healthRegen;
                    break;
                }
                if (k->type != HEALTH) {
                    bool foundActive = false;
                    for (std::list<Entity::Pickup>::iterator pickup = activePickups.begin(); pickup != activePickups.end(); pickup++) {
                        if (pickup->type == k->type) {
                            pickup->timer = k->powerupTimer;
                            foundActive = true;
                        }
                    }
                    if (!foundActive) {
                        activePickups.push_back(*k);
                    }
                }

                spawnedPickups.erase(k++);
                continue;
            }
        }

        modelMatrix = vm;
        modelMatrix *= transform2D::Translate(k->body.center.x, k->body.center.y);
        glm::mat3 backgroundMatrix = modelMatrix;


        modelMatrix = backgroundMatrix * transform2D::Scale(pickupStandardRadius, pickupStandardRadius);
        PickupRenderer(vm, k->body.center, glm::vec2(pickupStandardRadius, pickupStandardRadius), k->type);

        k++;
    }

    // obstacles
    std::vector<Rectangle>::iterator j = obstacles.begin();
    while (j != obstacles.end()) {
        modelMatrix = vm;
        modelMatrix *= transform2D::Translate(j->corner.x, j->corner.y);
        modelMatrix *= transform2D::Scale(j->width, j->height);
        RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);

        j++;
    }


    // border
    if (!minimap) {
        modelMatrix = vm;
        modelMatrix *= transform2D::Translate(map.x, map.y);
        modelMatrix *= transform2D::Scale(map.width, map.height);
        RenderMesh2D(meshes["barrier"], shaders["VertexColor"], modelMatrix);
    }

}

void Tema1::DrawCursor(glm::mat3 vm) {
    modelMatrix = vm;
    modelMatrix *= transform2D::Translate(mousePos.x, mousePos.y);
    modelMatrix *= transform2D::Scale(0.1f, 0.1f);
    modelMatrix *= transform2D::Rotate(glm::radians(angularStep));
    RenderMesh2D(meshes["cursor"], shaders["VertexColor"], modelMatrix);
}

void Tema1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();

    static glm::vec3 rgb = glm::vec3(0.5f, 0.5f, 0.5f);

    // end Game condition
    if (player.health <= 0) {
        if (!flag) {
            cout << "===========GAME OVER===========" << endl << "FINAL SCORE: " << score << endl;
            flag = true;
            window->ShowPointer();
        }
        viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
        SetViewportArea(viewSpace, rgb, true);
    }
    else {

        player.currentLookDir = glm::normalize(mousePos - player.pos);
        // rotate cursor
        angularStep += deltaTimeSeconds * 20;

        // timer update
        fireRateTimer + deltaTimeSeconds >= fireRate ? fireRateTimer = fireRate : fireRateTimer += deltaTimeSeconds;
        timer += deltaTimeSeconds;
        pickupTimer += deltaTimeSeconds;
        increaseDifficulty();

        // update visionBorder
        visionBorder.corner.x = logicSpace.x;
        visionBorder.corner.y = logicSpace.y;

        // enemy spawn timer update
        spawnTimer += deltaTimeSeconds;
        if (spawnTimer >= MaxTimeSpawn) {
            spawnTimer = 0;
            enemySpawner(maxSpeed);
        }

        if (pickupTimer >= 4) {
            pickupTimer = 0;
            pickupGenerator();
        }

        // Sets the screen area where to draw - whole window
        viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
        SetViewportArea(viewSpace, glm::vec3(0, 0, 0), true);

        visMatrix = glm::mat3(1);
        visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);
        DrawCursor(visMatrix);
        DrawScene(visMatrix, deltaTimeSeconds);
        
        
        float minimapResw = resolution.x / 4.5f;
        float minimapFactor = mapLogicSpace.height / mapLogicSpace.width;
        float borderscale = minimapResw * 1 / 70;

        // border mini-map
        viewSpace = ViewportSpace(0, 0, minimapResw + borderscale, minimapResw * minimapFactor + borderscale);
        SetViewportArea(viewSpace, glm::vec3(0.7f, 0, 0), true);

        // mini-map
        viewSpace = ViewportSpace(0, 0, minimapResw, minimapResw * minimapFactor);
        SetViewportArea(viewSpace, glm::vec3(0.8f, 0.8f, 0.8f), true);

        viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
        SetViewportArea(viewSpace, glm::vec3(0, 0, 0), false);
        visMatrix = glm::mat3(1);
        visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);

        DrawCursor(visMatrix);

        // mini-map draw
        viewSpace = ViewportSpace(0, 0, minimapResw, minimapResw * minimapFactor);
        SetViewportArea(viewSpace, glm::vec3(0.8f, 0.8f, 0.8f), false);
        glm::mat3 visMatrixMinimap = glm::mat3(1);
        visMatrixMinimap *= VisualizationTransf2DUnif(mapLogicSpace, viewSpace);
        DrawScene(visMatrixMinimap, deltaTimeSeconds, true);

    }
}

void Tema1::FrameEnd()
{
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
 
    if (window->KeyHold(GLFW_KEY_W) && (player.pos.y + player.radius) < map.y + map.height) {
        mainBody.center.y += playerSpeed * deltaTime;
        if (canMove()) {
            logicSpace.y += playerSpeed * deltaTime;
            player.pos.y += playerSpeed * deltaTime;
            mousePos.y += playerSpeed * deltaTime;
        }
    }

    if (window->KeyHold(GLFW_KEY_S) && (player.pos.y - player.radius) > map.y) {
        mainBody.center.y -= playerSpeed * deltaTime;
        if (canMove()) {
            logicSpace.y -= playerSpeed * deltaTime;
            player.pos.y -= playerSpeed * deltaTime;
            mousePos.y -= playerSpeed * deltaTime;
        }
    }

    if (window->KeyHold(GLFW_KEY_A) && (player.pos.x - player.radius) > map.x) {
        mainBody.center.x -= playerSpeed * deltaTime;
        if (canMove()) {
            logicSpace.x -= playerSpeed * deltaTime;
            player.pos.x -= playerSpeed * deltaTime;
            mousePos.x -= playerSpeed * deltaTime;
        }
    }

    if (window->KeyHold(GLFW_KEY_D) && (player.pos.x + player.radius) < map.x + map.width) {
        mainBody.center.x += playerSpeed * deltaTime;
        if (canMove()) {
            logicSpace.x += playerSpeed * deltaTime;
                player.pos.x += playerSpeed * deltaTime;
                mousePos.x += playerSpeed * deltaTime;
        }
    }

    mainBody.center = player.pos;
}


void Tema1::OnKeyPress(int key, int mods)
{

}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    glm::ivec2 resolution = window->GetResolution();
    mousePos.x = logicSpace.x - difference.x / 2 + (float)mouseX / resolution.x * ((float)logicSpace.width + difference.x);
    mousePos.y = logicSpace.y - difference.y / 2 + (1 - (float)mouseY / resolution.y) * ((float)logicSpace.height + difference.y);
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event

    float actualFireRate;
    if (playerStats.fireRate) {
        actualFireRate = 0.2f;
    }
    else
    {
        actualFireRate = fireRate;
    }
    float scale = 1;
    if (playerStats.cannon) {
        scale = 2;
    }

    if(IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) && fireRateTimer >= actualFireRate) {

        glm::vec2 corner = player.pos + player.currentLookDir * bulletSpawnDistance - (projectileSize.x / 2 * scale, projectileSize.y / 2 * scale);
        Projectile newProjectile = Projectile(Rectangle(corner, projectileSize.x * scale, projectileSize.y * scale), player.currentLookDir, playerBulletDamage, FRIENDLY);
        projectiles.push_back(newProjectile);


        if (playerStats.shotgun) {
            glm::vec3 originalTrajectory = glm::vec3(newProjectile.trajectory, 0);
            newProjectile.trajectory = glm::normalize(transform2D::Rotate(glm::radians(30.0f)) * originalTrajectory);
            projectiles.push_back(newProjectile);
            newProjectile.trajectory = glm::normalize(transform2D::Rotate(glm::radians(-30.0f)) * originalTrajectory);
            projectiles.push_back(newProjectile);
        }
        fireRateTimer = 0;
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
    UpdateDiff();
}