# MacOS 
# brew install glew & glfw & assimp & freeimage
# cmake -G "Xcode" ..

# Xcode 
# 1. Preference->Locations->Custom Path, add GLEW_INCLUDE\GLEW_LIB...
# 2. choose Build Settings->Search Path(header and library)
# 3. choose Build Phases->Link Libraries and Libraries Phases->add related libraries


### Use OpenGL 4.1 version instead of version 2.1
## https://www.glfw.org/faq.html#41---how-do-i-create-an-opengl-30-context
## https://developer.apple.com/library/archive/documentation/GraphicsImaging/Conceptual/OpenGL-MacProgGuide/opengl_pg_concepts/opengl_pg_concepts.html#//apple_ref/doc/uid/TP40001987-CH208-SW1
# glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
# glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
# glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
# glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
