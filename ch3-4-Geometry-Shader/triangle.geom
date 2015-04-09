#version 430 core

layout (triangles) in;     //进入的是三角形图元
layout (points, max_vertices = 3) out;  //输出是点模式，三个点进，三个点出

void main(void)
{
   for (int i = 0; i != gl_in.length(); ++i)  //当前图元的遍历
   {
     gl_Position = gl_in[i].gl_Position;   //简单的传递
	 EmitVertex();  //生成当前顶点
   }
   EndPrimitive();    //结束当前图元
}