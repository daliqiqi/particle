#ifndef PARTICLE_H
#define PARTICLE_H
#define M_PI        3.14159265358979323846
#define DEGTORAD(d)	((d * (double)M_PI) / 180.0f);
#define RADTODEG(r)	((r * 180.0f) /(double)M_PI);

class mVector3d
{
public:
    mVector3d():x(0), y(0), z(0){}  //三种初始化
    mVector3d(double x1, double y1, double z1):x(x1), y(y1), z(z1){}
    mVector3d(const mVector3d &v){ x = v.x;  y = v.y;  z = v.z; }//这种初始化看不懂
    ~mVector3d(){}
    void operator=(const mVector3d &v){  x = v.x;  y = v.y;  z = v.z; }
    mVector3d operator+(const mVector3d &v){  return mVector3d(x+v.x, y+v.y, z+v.z);  }
    mVector3d operator-(const mVector3d &v){  return mVector3d(x-v.x, y-v.y, z-v.z);  }
    mVector3d operator+(double d){  return mVector3d(x+d, y+d, z+d);  }
    mVector3d operator-(double d){  return mVector3d(x-d, y-d, z-d);  }
    mVector3d operator*(double d){  return mVector3d(x*d, y*d, z*d);  }  //函数符号重载的写法
public:
    double x, y, z;
};

class mVector4d
{
public:
    mVector4d():r(0), g(0), b(0), a(0){}
    mVector4d(double r, double g, double b, double a):r(r), g(g), b(b), a(a){}
    ~mVector4d(){}
    void operator=(const mVector4d &v){  r = v.r;  g = v.g;  b = v.b; a = v.a; }
    mVector4d operator+(const mVector4d &v){  return mVector4d(r+v.r, g+v.g, b+v.b, a+v.a);  }
    mVector4d operator-(const mVector4d &v){  return mVector4d(r-v.r, g-v.g, b-v.b, a-v.a);  }
    mVector4d operator+(double d){  return mVector4d(r+d, g+d, b+d, a+d);  }
    mVector4d operator-(double d){  return mVector4d(r-d, g-d, b-d, a-d);  }
    mVector4d operator*(double d){  return mVector4d(r*d, g*d, b*d, a*d);  }
public:
    double r, g, b, a;
};

struct tParticle
{
    tParticle   *prev,*next;			// 粒子间以链表形式连接
    mVector3d	pos;					// 当前位置
    mVector3d	prevPos;				// 先前位置
    mVector3d	direction;				// 当前速度方向
    int		    restLife;				// 剩余存活时间
    mVector4d	currentColor;			// 当前颜色
    mVector4d	prevColor;				// 先前颜色
    mVector4d	deltaColor;				// 颜色变化率
};

class tEmitter
{
public:
    tEmitter();
    ~tEmitter();
    void addParticle();
    void updateParticle(tParticle *particle);
public:
    mVector3d	pos;							// 发射器位置
    mVector3d    direction;						// 发射器喷射主方向
    //通过球坐标系随机产生指定方向范围内的速度向量
    double		voy_max, voy_min;				// 粒子初始发射速度与正y轴的夹角范围
    double		vox_max, vox_min;				// 粒子初始发射速度与正X轴的夹角范围
    double		speed_max, speed_min;			// 粒子初始速度变化范围
    // 粒子设置信息
    tParticle   *ptcPool;
    tParticle	*ptc;							// 指向当前活跃粒子链表
    int			totalPtc;						// 粒子库粒子总数
    int			curPtc;							// 当前粒子数
    int			pfPtc_max, pfPtc_min;			// 每一帧添加粒子数变化范围
    int			life_max, life_min;				// 粒子初始生命值变化范围
    mVector4d	startColor_max, startColor_min;	// 初始颜色变化范围
    mVector4d	endColor_max, endColor_min;		// 消失颜色变化范围
    //重力设置信息
    mVector3d	force;							// 三坐标轴速度变化率
};

class ParticleSystem
{
public:
    void initEmitter();
    void updateEmitter();
    void renderEmitter(bool antiAlias);

public:
    tEmitter *emitter;
};

void TransAndRotate(mVector3d pos, mVector3d destiny);



#endif // PARTICLE_H

