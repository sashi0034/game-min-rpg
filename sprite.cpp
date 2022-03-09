#define _USE_MATH_DEFINES

#include "sprite.h"
#include <math.h>
#include <random>
#include <iostream>




std::vector<Sprite*> Sprite::sprites = std::vector<Sprite*>();

void Sprite::Init()
{
    sprites = std::vector<Sprite*>();
}


void Sprite::End()
{
    Sprite::DisposeAll();
}


Sprite::Sprite() : Sprite(nullptr, 0, 0, 0, 0)
{}
Sprite::Sprite(Graph* image) : Sprite(image, 0, 0, 0, 0)
{
    int x = 0, y = 0;
    DxLib::GetGraphSize(image->GetHandler(), &x, &y);
    this->width = x;
    this->height = y;
}
Sprite::Sprite(Graph* image, int u, int v, int w, int h)
{
    this->image = image;
    this->u = u;
    this->v = v;
    this->width = w;
    this->height = h;
    this->drawingMethod = Sprite::DrawingKind::Rough;

    sprites.push_back(this);
}



void Sprite::SetFlip(bool isFlip)
{
    this->isFlip = isFlip;
}


void Sprite::SetImage(Graph* image)
{
    this->image = image;
}
void Sprite::SetImage(int u, int v)
{
    this->u = u;
    this->v = v;
}
void Sprite::SetImage(int u, int v, int width, int height)
{
    this->SetImage(u, v);
    this->width = width;
    this->height = height;
}
void Sprite::SetImage(Graph* image, int u, int v, int width, int height)
{ 
    this->SetImage(image);
    this->SetImage(u, v, width, height);
}


void Sprite::SetXY(double x, double y)
{
    this->x = x;
    this->y = y;
}
void Sprite::SetZ(double z)
{
    this->z = z;
}
double Sprite::GetZ()
{
    return this->z;
}


void Sprite::GetScreenXY(int* x, int* y)
{
    double x1, y1;
    this->GetLinkDifferenceXY(&x1, &y1);

    *x = int(this->x + x1);
    *y = int(this->y + y1);
}


void Sprite::SetRotationDeg(double deg)
{
    this->rotationRad = (deg / 180.0) * M_PI;
}
void Sprite::SetRotationRad(double rad)
{
    this->rotationRad = rad;
}


void Sprite::SetBelong(std::any instance)
{
    this->belong = instance;
}

std::any Sprite::GetBelong()
{
    return this->belong;
}

void Sprite::SetLinkXY(const Sprite* linkSpr)
{
    this->linkXY = linkSpr;
}


void Sprite::GetLinkDifferenceXY(double* x, double* y)
{
    const Sprite* linkSpr = this->linkXY;
    if (linkSpr != nullptr)
    {
        double x1, y1;
        const_cast<Sprite*>(linkSpr)->GetLinkDifferenceXY(&x1, &y1);
        *x = linkSpr->x + x1;
        *y = linkSpr->y + y1;
        return;
    }
    else
    {
        *x = 0;
        *y = 0;
        return;
    }
}


void Sprite::SetBlend(int blendMode, int blendPal)
{
    this->SetBlendMode(blendMode);
    this->SetBlendPal(blendPal);
}
void Sprite::SetBlendMode(int blendMode)
{
    this->blendMode = blendMode;
}
void Sprite::SetBlendPal(int blendPal)
{
    this->blendPal = blendPal;
}


void Sprite::SetUpdateMethod(void (*updateMethod)(Sprite* hSp))
{
    this->updateMethod = updateMethod;
}
void Sprite::SetDrawingMethod(void (*drawingMethod)(Sprite* hSp, int hX, int hY))
{
    this->drawingMethod = drawingMethod;
}
void Sprite::SetDestructorMethod(void (*destructorMethod)(Sprite* hSp))
{
    this->destructorMethod = destructorMethod;
}


void Sprite::SetProtect(bool isProtect)
{
    this->isProtect = isProtect;
}

void Sprite::Dispose(Sprite* spr)
{
    if (spr == nullptr) return;
    if (spr->destructorMethod != nullptr)
    {
        spr->destructorMethod(spr);
    }

    spr->sprites[findIndex(spr)] = nullptr;
}


void Sprite::Dispose(Sprite* spr, bool isUnprotectOnly)
{
    if (isUnprotectOnly)
    {
        if (!spr->isProtect) Dispose(spr);
    }
    else
    {
        Dispose(spr);
    }
}

void Sprite::DisposeAll()
{
    DisposeAll(false);
}
void Sprite::DisposeAll(bool isProtectOnly)
{
    for (Sprite* spr : sprites)
    {
        if (isProtectOnly)
        {
            if (!spr->isProtect) Dispose(spr);
        }
        else
        {
            Dispose(spr);
        }
    }
    collectGarbageSprites();
}

int Sprite::findIndex(Sprite* spr)
{
    auto size = sprites.size();
    if (size == 0) return -1;

    auto iter = std::find(sprites.begin(), sprites.end(), spr);
    size_t index = std::distance(sprites.begin(), iter);
    if (index == size)
    {
        return -1;
    }
    return index;
}
void Sprite::collectGarbageSprites()
{
    while (true)
    {
        int index = findIndex(nullptr);
        if (index == -1) break;
        sprites.erase(sprites.begin() + index);
    }
}




void Sprite::UpdateAll()
{
    for (Sprite* spr : sprites)
    {
        if (spr != nullptr)
        {
            if (spr->updateMethod != nullptr) spr->updateMethod(spr);
        }
    }
    collectGarbageSprites();
}

/// <summary>
/// ‘SSprite‚ğ•`‰æ‚·‚é
/// image‚ªnullptr‚ÌSprite‚ÍdrawingMethod‚ªŒÄ‚Î‚ê‚È‚¢‚Ì‚Å’ˆÓ‚·‚é‚±‚Æ
/// </summary>
void Sprite::DrawingAll()
{
    std::sort(sprites.begin(), sprites.end(),
        [](Sprite* left, Sprite* right) { return left->GetZ() > right->GetZ(); });

    for (Sprite* spr : sprites)
    {
        if (spr != nullptr)
        {
            if (spr->image == nullptr) continue;

            int x, y;
            double x1, y1;
            spr->GetLinkDifferenceXY(&x1, &y1);

            x1 = (spr->x + x1);
            y1 = (spr->y + y1);

            x1 *= ROUGH_SCALE;
            y1 *= ROUGH_SCALE;

            x = int(x1);
            y = int(y1);

            DxLib::SetDrawBlendMode(spr->blendMode, spr->blendPal);

            if (spr->drawingMethod != nullptr)
            {
                spr->drawingMethod(spr, x, y);
            }
        }
    }
    DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

}




void Sprite::DrawingKind::Rough(Sprite* hSpr, int hX, int hY)
{
    Draw(hSpr, hX, hY, ROUGH_SCALE);
}
void Sprite::DrawingKind::Twice(Sprite* hSpr, int hX, int hY)
{
    Draw(hSpr, hX, hY, 2);
}
void Sprite::DrawingKind::DotByDot(Sprite* hSpr, int hX, int hY)
{
    Draw(hSpr, hX, hY, 1);
}
void Sprite::DrawingKind::Draw(Sprite* hSpr, int x, int y, int scale)
{
    hSpr->image->DrawGraph(x, y, hSpr->u, hSpr->v, hSpr->width, hSpr->height, scale, hSpr->rotationRad, hSpr->isFlip);
}













