#ifndef PLAYER_H
#define PLAYER_H

#define GRAVITY 0.5f
#define JUMP_FORCE (-15.0f)
#define SPEED 5.0f

class Player {
    float x, y;
    float velocityY;
    bool isJumping;

public:
    Player()
        : x(0)
          , y(0)
          , velocityY(0)
          , isJumping(true) {
    }

    void update(const bool moveLeft, const bool moveRight, const bool shouldJump) {
        // Movimento horizontal
        if (moveLeft) x -= SPEED;
        if (moveRight) x += SPEED;

        if (x < 0) x = 0;

        // Pulo e gravidade
        if (shouldJump && !isJumping) {
            velocityY = JUMP_FORCE;
            isJumping = true;
        }

        y += velocityY;
        velocityY += GRAVITY;

        // Verificação básica de colisão com o chão
        // TODO: Refatorar para usar uma estrutura de colisão mais robusta
        if (y > 1080) {
            y = 1080;
            velocityY = 0;
            isJumping = false;
        }
    }

    [[nodiscard]] float getX() const { return x; }
    [[nodiscard]] float getY() const { return y; }
    [[nodiscard]] static float getSpeed() { return SPEED; }

    void resetJump() { isJumping = false; }
};

#endif //PLAYER_H
