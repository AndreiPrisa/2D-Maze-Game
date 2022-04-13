# pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema1/Entity.h"

using namespace Entity;

class Tema1 : public gfxc::SimpleScene
{
    public:
        Tema1();
        ~Tema1();

        void Init() override;

        struct ViewportSpace
        {
            ViewportSpace() : x(0), y(0), width(1), height(1) {}
            ViewportSpace(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        struct LogicSpace
        {
            LogicSpace() : x(0), y(0), width(1), height(1) {}
            LogicSpace(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {}
            float x;
            float y;
            float width;
            float height;
        };

        struct Map
        {
            Map() : x(0), y(0), width(100), height(100) {}
            Map(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {}
            float x;
            float y;
            float width;
            float height;
        };

        struct Player
        {
            Player() : pos(0),
                health(100),
                headColor(0.5, 0, 0.5), 
                handColor(1, 0, 0.5), 
                handDistance(3.40f), 
                handRadius(0.10f), 
                radius(1), 
                defaultLookDir(1, 0), 
                currentLookDir(1, 0) {}
            Player(glm::vec2 pos, float health,
                glm::vec3 headColor = glm::vec3(0.5, 0, 0.5),
                glm::vec3 handColor = glm::vec3(1, 0, 0.5),
                glm::vec2 handDistance = glm::vec2(0, 0.4f),
                float handRadius = 0.10f, float radius = 0.5f)
                : pos(pos), health(health), headColor(headColor), handColor(handColor), handDistance(handDistance), handRadius(handRadius), radius(radius), defaultLookDir(1, 0), currentLookDir(1, 0) {}
            glm::vec2 pos;
            float health;
            glm::vec3 headColor;
            glm::vec3 handColor;
            glm::vec2 handDistance;
            float handRadius;
            float radius;
            glm::vec2 defaultLookDir;
            glm::vec2 currentLookDir;
        };

        struct Stats {
            Stats() : fireRate(false), shotgun(false), cannon(false) {}
                bool fireRate;
                bool shotgun;
                bool cannon;
        };

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void Tema1::PickupRenderer(glm::mat3 vm, glm::vec2 translate, glm::vec2 scale, int type);

        bool canMove();
        void DrawScene(glm::mat3 vm, float deltaTimeSeconds, bool minimap = false);

        // Visualization
        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);
        void UpdateDiff();

        void enemySpawner(float maxSpeed, float minSpeed = 4);
        void pickupGenerator();
        bool outOfMap(Rectangle body);
        bool outOfMap(Circle body);
        void increaseDifficulty();
        void Tema1::DrawCursor(glm::mat3 vm);

    protected:

        float length;
        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        glm::mat3 modelMatrix, visMatrix;


        // Player
        Player player;
        float playerSpeed;
        Circle mainBody;
        Rectangle visionBorder;
        Stats playerStats;

        int maxHealth;
        float fireRate;
        float fireRateTimer;
        float bulletSpawnDistance;
        float playerBulletDamage;

        // Pickups
        std::list<Entity::Pickup> spawnedPickups;
        std::list<Entity::Pickup> activePickups;
        float pickupStandardRadius;
        float activeTime;
        float healthRegen;
        float pickupTimer;

        // timers
        float timer;

        // HUD
        Rectangle maxHealthBar;
        Rectangle currentHealthBar;
        float score;
        float maxScore;
        float barDistance;

        Rectangle maxScoreBar;
        Rectangle currentScoreBar;

        Rectangle maxPowerBar;

        // Map
        Map map;
        LogicSpace mapLogicSpace;
        float unit;

        // Mouse
        glm::vec2 mousePos;
        glm::vec2 difference;
        float angularStep;

        // obstacles
        std::vector<Entity::Rectangle> obstacles;

        // projectiles
        std::list<Entity::Projectile> projectiles;
        glm::vec2 projectileSize;
        float projectileSpeed;
        float maxDistance;

        // enemies
        std::list<Entity::Enemy*> enemies;
        Entity::Enemy normalEnemy;
        Entity::Gunner normalGunner;
        float maxSpeed;
        int noOfSpawns;
        float spawnTimer;
        float MaxTimeSpawn;

        // border thickness
        float thickness;

        bool flag;
};