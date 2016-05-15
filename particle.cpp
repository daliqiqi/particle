#include "Particle.h"
#include <math.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//0到1随机小数产生器
double Random()
{
    return (double)rand()/ (double)RAND_MAX;
}

//确定粒子初始速度方向，使用球坐标系，vox为速度在xoz平面上的投影与x正轴的夹角，voy为速度与y正轴的夹角
void RandomDirection(double vox, double voy, mVector3d *direction)
{
    direction->x = (double)(sin(voy) * sin(vox));
    direction->y = (double)cos(voy);
    direction->z = (double)(sin(voy) * cos(vox));
}

tEmitter::tEmitter()
{
}
tEmitter::~tEmitter()
{
    if(ptcPool)
        delete ptcPool;
}
//粒子添加器
void tEmitter::addParticle()
{
    tParticle *particle;
    mVector4d startColor, endColor;
    double voy, vox, speed;

    if (ptcPool != NULL && curPtc < totalPtc)
    {
        //从粒子库中取出一个粒子链入当前活跃粒子链表中
        particle = ptcPool;//池中的一个元素
        ptcPool = ptcPool->next;

        if (ptc != NULL)
            ptc->prev = particle;
        particle->next = ptc;
        particle->prev = NULL;
        ptc = particle;

        //在一个半径为0.01的球面内的某片曲面中随机取一个位置给粒子作初始点
        double degreeY = DEGTORAD(120.0 + 60.0 * Random());//begtorad
        double degreeXZ = DEGTORAD(360.0 * Random());
        particle->pos = mVector3d(0.001 * sin(degreeY) * cos(degreeXZ), 0.001 * (1 - cos(degreeY)), 0.001 * sin(degreeY) * sin(degreeXZ));
        particle->prevPos = particle->pos;	// 用于线段模式

        // 随机产生初始速度向量
        voy = (voy_max - voy_min) * Random();
        vox =  (vox_max - vox_min) * Random();;
        RandomDirection(vox,voy,&particle->direction);
        speed = speed_min + (speed_max  - speed_min) * Random();
        particle->direction = particle->direction * speed;

        // 随机产生粒子初始颜色及终止颜色
        startColor = startColor_min + (startColor_max - startColor_min) * Random();
        endColor =  endColor_min + (endColor_max - endColor_min) * Random();
        particle->currentColor = startColor;

        // 随机产生粒子生命值，数值表示粒子存活帧数
        particle->restLife = life_min + (life_max - life_min) * Random();

        // 计算粒子颜色变化率
        particle->deltaColor= (endColor - startColor) *(1.0 / (double)particle->restLife);

        curPtc++;	//发射器当前粒子数加1
    }
}

//粒子更新器
void tEmitter:: updateParticle(tParticle *particle)
{
    if (particle != NULL && particle->restLife > 0)
    {
        particle->prevPos = particle->pos;
        particle->pos = particle->pos + particle->direction;
        particle->direction = particle->direction + force;

        particle->prevColor = particle->currentColor;
        //if(particle->restLife < 20)
            particle->currentColor = particle->currentColor + particle->deltaColor;

        particle->restLife--;
    }
    else if (particle != NULL && particle->restLife == 0)
    {
        //修改当前粒子的前后粒子的指针
        if (particle->prev != NULL)
            particle->prev->next = particle->next;
        else
            ptc = particle->next;
        if (particle->next != NULL)
            particle->next->prev = particle->prev;
        //回收至粒子库
        particle->next = ptcPool;
        ptcPool = particle;
        curPtc--;
    }
}

//发射器初始化，包括粒子库初始化
void ParticleSystem::initEmitter()
{
    srand((unsigned)time( NULL ));
    emitter = new tEmitter;
    emitter->totalPtc = 40000;
    emitter->ptcPool = (tParticle *)malloc(emitter->totalPtc * sizeof(tParticle));
    for (int i = 0; i < emitter->totalPtc - 1; i++)
    {
        emitter->ptcPool[i].next = &emitter->ptcPool[i + 1];
    }

    emitter->ptcPool[emitter->totalPtc - 1].next = NULL;

    //可改成外部设置
    emitter->pos = mVector3d(0.0f, 0.0f, 0.0f);
    float m=Random();
    emitter->direction = mVector3d(m, m, m);

    emitter->voy_max =	DEGTORAD(360.0f);
    emitter->voy_min =	DEGTORAD(0.0f);
    emitter->vox_max =	DEGTORAD(360.0f);
    emitter->vox_min =  DEGTORAD(0.0f);
    emitter->speed_max =  0.006f;
    emitter->speed_min =  0.001f;

    emitter->curPtc	= 0;
    emitter->pfPtc_max	= 2000;
    emitter->pfPtc_min	= 1000;
    emitter->life_max = 45;
    emitter->life_min = 25;

    emitter->startColor_max = mVector4d(1.0f, 0.8f, 0.8f, 0.8f);
    emitter->startColor_min = mVector4d(0.9f, 0.72f, 0.8f, 0.8f);
    emitter->endColor_max = mVector4d(0.0f, 1.0f, 0.0f, 0.0f);
    emitter->endColor_min = mVector4d(0.0f, 0.8f, 0.0f, 0.0f);

    emitter->force = mVector3d(0.0f, 0.0006f, 0.0f);
    emitter->ptc	 = NULL;					// NULL TERMINATED LINKED LIST
}

//更新发射器
void ParticleSystem::updateEmitter()
{
    tParticle *particle, *next;
    if (emitter != NULL)
    {
        if (emitter->ptc != NULL)
        {
            particle = emitter->ptc;
            while (particle)
            {
                next = particle->next;
                emitter->updateParticle(particle);
                particle = next;
            }
        }
        int emits = emitter->pfPtc_min + (emitter->pfPtc_max - emitter->pfPtc_min) * Random();
        for (int i = 0; i < emits; i++)
            emitter->addParticle();
    }
}

//绘制粒子系统，传入参数控制绘制模式，若为1则是线模式，为0则是点模式
void ParticleSystem::renderEmitter(bool antiAlias)
{
    updateEmitter();
    tParticle *particle = emitter->ptc;
    glPointSize(2.0);
    glLineWidth(2.0);
    if (antiAlias)
        glBegin(GL_LINES);
    else
        glBegin(GL_POINTS);
    while(particle)
    {
        if (antiAlias)
        {
            glColor4f(particle->prevColor.r, particle->prevColor.g, particle->prevColor.b,  particle->prevColor.a);
            glVertex3f(particle->prevPos.x,particle->prevPos.y,particle->prevPos.z);
        }
        glColor4f(particle->currentColor.r, particle->currentColor.g, particle->currentColor.b,  particle->currentColor.a);
        glVertex3f(particle->pos.x,particle->pos.y,particle->pos.z);
        particle = particle->next;
    }
    glEnd();
}

//通过给定的喷射方向destiny确定旋转轴以及旋转角度degree并作旋转，默认喷射方向是（0,1,0）
void TransAndRotate(mVector3d pos, mVector3d destiny)
{
    glTranslatef(pos.x, pos.y, pos.z);
    double axis[3];
    //axis为向量destiny与（0,1,0）的叉积
    axis[0] = destiny.z;
    axis[1] = 0;
    axis[2] = -destiny.x;
    double degree = RADTODEG(acos(destiny.y/sqrt(destiny.x*destiny.x + destiny.y*destiny.y + destiny.z*destiny.z)));
    glRotated(degree, axis[0], axis[1], axis[2]);
}
