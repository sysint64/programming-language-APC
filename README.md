# README #

Новый язык программирование

В начале 2012го года начал писать свой язык программирования just for fun :).

Некоторые особенности нового яп: 

массивы определяются таким образом: `[2 x int]`; `[2 x [5 x float]]`. Так же есть новый тип данных для работы с SIMD - векторы, определяются таким образом: `<4 x int>`; `<16 x float>`; двумерных векторов не бывает. Так например намного легче раюотать с SIMD ежели в C++, к примеру сложение 2х векторов: 

```ruby
def main() 
    a, b : <4 x int>; 
    a += b; // выполняется сложение через SIMD 
end; 
```

этот вариант в 4 раза быстрее работает чем: 

```ruby
def main() 
    a, b : [4 x int]; 

    a[0] += b[0]; a[1] += b[1]; 
    a[2] += b[2]; a[3] += b[3]; 
end; 
```

для инициализации массивов можно использовать вот такую шляпу: 
`arr = [2, 4, 6]`; или `arr = [2, 4, ...]`; или `arr = [..., 4, 6]`; там где стоит `...` автоматически подставит значения из массива, бывает удобно в некоторых случаях, так же и с векторами только вместо квадратных скобок нужно использовать угловатые.
Из особенностей так же можно выделить возможность создавать перегрузку операторов. Пример перегрузки для массивов: 

```ruby
alias vec3i = [3 x int]; 

def binary+ (lhs, rhs : vec3i) : vec3i inline swap 
    res : vec3i; 

    res[0] = lhs[0]+rhs[0];
    res[1] = lhs[1]+rhs[1];
    res[2] = lhs[2]+rhs[2];
    
    return res; 
end; 

def main() 
    a, b : vec3i; 
    a += b; // в данном случае ошибки не будет, а вызовется перегруженный оператор, который сложит по элементно массивы 
end. 
```

с унарными так же, также есть `оператор []`, `assign` и `assign[]`; так к примеру можно сделать хэштаблицу с обращением к элементам по строке к примеру `map['test'] := 10`; 
так например тип `string` - это просто структура такого вида: 

```ruby
struct string 
    data   : [char]; 
    length : int; 
end; 
```

Для генерации и оптимизации исполняемого файла генерируется LLVMIR (промежуточное представление кода LLVM) и с помощью LLVM оптимизируется и генерируется ASM и исполняемый файлы. 

# Примеры кода #

## Перемножение 2х матриц 4x4 с SIMD векторизацией: ##


```ruby
def mmul (const m1, m2 : mat4f) : mat4 inline 
    res : mat4;

    for i : int = 0, 3 do 
        ci : <4 x float> = m1[i][0]*m2[0]; 

        for j : int = 0, 3 do 
            ci += m[i][j]*m2[j] 
    end; 

    res[i] = ci; 
end; 
```

## перевод числа в строку: ##

```ruby
def int2str (val : int) : string 
    x,v,l : int = 0; 
    v = val; 
    
    if (val == 0) then
        return "0";

    while (v != 0) do
        l++;
        v /= 10;
    end; 

    len : int = l; 
    string res = new string(len+1); 

    while (l != 0) do 
        x = val % 10; l--; 
        res.data[l] = x+'0'; 
        val /= 10; 
    end; 

    data[len] = #0; 
    return res; 
end; 
```

## Декодирования UTF8: ##

```ruby
def decodeUTF8 (uc : wchar) : string 
    res : string; 
    
    if (uc == 0x0040) then
        return "";
        
    if (uc >= 0xD800) && (uc <= 0xDFFF) then
        return "";
    
    if (uc > 0x1) && (uc <= 0x007F) then

        res += Char(uc); 
        
    elsif (uc > 0x007F) && (uc <= 0x07FF) then 
        
        b1 : char = 0xC0 | (uc & 0x7C0) >> 6; 
        b2 : char = 0x80 | (uc & 0x3F); 

        res += b1; res += b2; 
        
    elsif (uc > 0x07FF) && (uc <= 0xFFFF) then 
        
        b1 : char = 0xE0 | (uc & 0xF000) >> 12; 
        b2 : char = 0x80 | (uc & 0xFC0) >> 6; 
        b3 : char = 0x80 | (uc & 0x3F); 
              
        res += b1; res += b2; 
        res += b3; 
        
    elsif (uc > 0xFFFF) && (uc <= 0x10FFFF) then 
        
        b1 : char = 0xF0 | (uc & 0x1C00000) >> 18; 
        b2 : char = 0x80 | (uc & 0x3F000) >> 12; 
        b3 : char = 0x80 | (uc & 0xFC0) >> 6; 
        b4 : char = 0x80 | (uc & 0x3F); 

        res += b1; res += b2; 
        res += b3; res += b4; 
        
    end; 

    return res; 
end; 
```

## OpenGL Графика ##

```ruby
include "sys.apc";
include "math.apc";
include "glfw3.apc";
include "opengl.apc";
include "string.apc";
//
def main()
    glfwInit();

    win : ^GLFWwindow = glfwCreateWindow (640, 480, "Hello World with OpenGL :)", nil, nil);
    
    glfwMakeContextCurrent (win);
    glfwSwapInterval (0);

    t0 : float = glfwGetTime();
    fps, frames : float = 0;

    while (glfwWindowShouldClose(win) != 1) do
        t : float = glfwGetTime();

        if ((t-t0) > 1) || (frames == 0) then
            fps = frames / (t-t0);
            t0 = t; frames = 0;
        end;

        frames++;

        glViewport (0, 0, width, height);

        // Clear color buffer
        glClearColor (1, 1, 1, 0.0);
        glClear (GL_COLOR_BUFFER_BIT);

        // Select and setup the projection matrix
        glMatrixMode (GL_PROJECTION);
        glLoadIdentity();
        gluPerspective (65.0, 1.33, 1.0, 100.0);

        // Select and setup the modelview matrix
        glMatrixMode (GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt (0.0,  1.0, 0.0,   // Eye-position
                   0.0, 20.0, 0.0,   // View-point
                   0.0,  0.0, 1.0);  // Up-vector

        // Draw a rotating colorful triangle
        glRotatef (0.3 + 100.0, 0.0, 0.0, 1.0);
        
        glBegin (GL_TRIANGLES);
            glColor3f  ( 1.0, 0.0,  0.0);
            glVertex3f (-5.0, 0.0, -4.0);
            glColor3f  ( 0.0, 1.0,  0.0);
            glVertex3f ( 5.0, 0.0, -4.0);
            glColor3f  ( 0.0, 0.0,  1.0);
            glVertex3f ( 0.0, 0.0,  6.0);
        glEnd;

        glFlush;

        glfwSwapBuffers (win);
        glfwPollEvents();
    end;

    glfwDestroyWindow (win);
    glfwTerminate();
end;
```

### Результат ###

![868c99bb8ff3a15dceab2f7968d453ef.png](https://bitbucket.org/repo/9egyB6/images/1882587105-868c99bb8ff3a15dceab2f7968d453ef.png)

# Автор #

Кабылин Андрей Николаевич
