#ifndef ENGINE_LAW_H
#define ENGINE_LAW_H



class Engine_Law
{
public:
     Engine_Law(int Mode,double &Gn1r,double &Gn2r,double &GZcl,double &GZch,double &GT4);//输入时要求给出6组试给值
     //Gn1r：低压转子相对转速试给值
     //Gn2r：高压转子相对转速试给值
     //GZcl：低压压气机压比试给值
     //GZch：高压压气机压比试给值
     //GT4：涡轮前温度试给值
};

#endif // ENGINE_LAW_H
