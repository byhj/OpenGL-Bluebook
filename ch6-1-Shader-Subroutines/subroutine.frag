#version 430 core

subroutine vec4 MySubroutine(vec4 param);

subroutine(MySubroutine) vec4 myFunction1(vec4 param)
{
   return param * vec4(1.0, 0.25, 0.25, 1.0);
}

subroutine(MySubroutine) vec4 myFunction2(vec4 param)
{
   return param * vec4(0.25, 0.25, 1.0, 1.0);
}

subroutine uniform MySubroutine subroutineUniform;

out vec4 color;

void main()
{
   color = subroutineUniform(vec4(1.0));  //sub(param)
}