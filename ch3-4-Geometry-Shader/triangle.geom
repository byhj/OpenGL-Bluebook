#version 430 core

layout (triangles) in;     //�������������ͼԪ
layout (points, max_vertices = 3) out;  //����ǵ�ģʽ������������������

void main(void)
{
   for (int i = 0; i != gl_in.length(); ++i)  //��ǰͼԪ�ı���
   {
     gl_Position = gl_in[i].gl_Position;   //�򵥵Ĵ���
	 EmitVertex();  //���ɵ�ǰ����
   }
   EndPrimitive();    //������ǰͼԪ
}