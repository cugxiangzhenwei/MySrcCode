#ifndef _PARTICLE_H
#define _PARTICLE_H

struct snow
{
	float x;
	float y;
	float speed; //速度
	int number;  //粒子图像编号
};
class CParticle
{
private:
	int m_number; //数量
	struct snow *m_pSnow;  //雪花
	CImage m_snowMap[7]; //七种雪花图像
public:
	CParticle(int number);
	~CParticle();

public:
	void Init();  //初始化粒子
	void Draw(CDC &cDC);  //绘制粒子
	void Update(float time);//更新粒子
};

#endif