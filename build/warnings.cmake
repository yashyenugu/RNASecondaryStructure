if (MSVC)
    add_definitions (-D_CRT_SECURE_NO_WARNINGS)
    add_compile_options (/wd4127)

else ()
    add_compile_options (-Wall -Wextra)
    
endif()