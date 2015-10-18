# created by @afable

c++ = g++
cflags = -Wall -g
frameworks = -framework OpenGL -framework GLUT
objs = main.o raytrace.o tracer.o lowlevel.o shader.o OSUObject.o

all:  $(objs)
	$(c++) $(objs) $(frameworks)

%.o : %.cpp
	$(c++) $(cflags) -c $<

clean:
	rm -f $(objs)
	rm -f a.out







