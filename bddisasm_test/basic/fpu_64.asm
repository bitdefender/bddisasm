    bits 64
    
    fadd           qword [rbx]     
    fmul           qword [rbx]     
    fcom           qword [rbx]     
    fcomp          qword [rbx]     
    fsub           qword [rbx]     
    fsubr          qword [rbx]     
    fdiv           qword [rbx]     
    fdivr          qword [rbx]     
    fadd           st7   
    fmul           st7   
    fcom           st7   
    fcomp          st7   
    fsub           st7   
    fsubr          st7   
    fdiv           st7   
    fdivr          st7   

    fld            qword [rbx]     
    fst            qword [rbx]     
    fstp           qword [rbx]     
    fldenv         [rbx]           
    fldcw          [rbx]            
    fnstenv        [rbx]           
    fnstcw         [rbx]            

    fnop                      
    fchs                      
    fabs                      
    ftst                      
    fxam                      
    fld1                      
    fldl2t                    
    fldl2e                    
    fldpi                     
    fldlg2                    
    fldln2                    
    fldz                      
    f2xm1                     
    fyl2x                     
    fptan                     
    fpatan                    
    fxtract                   
    fprem1                    
    fdecstp                   
    fincstp                   
    fprem                     
    fyl2xp1                   
    fsqrt                     
    fsincos                   
    frndint                   
    fscale                    
    fsin                      
    fcos                      
    fld            st7   
    fxch           st7   

    fiadd          dword [rbx]      
    fimul          dword [rbx]      
    ficom          dword [rbx]      
    ficomp         dword [rbx]      
    fisub          dword [rbx]      
    fisubr         dword [rbx]      
    fidiv          dword [rbx]      
    fidivr         dword [rbx]      


    fucompp                   
    fcmovb         st7   
    fcmove         st7   
    fcmovbe        st7   
    fcmovu         st7   

    fild           qword [rbx]      
    fisttp         qword [rbx]      
    fist           dword [rbx]      
    fistp          qword [rbx]      
    fld            qword [rbx]     
    fstp           qword [rbx]     
    fnop                      
    fndisi                    
    fnclex                    
    fninit                    
    fnop                      
    fcmovnb        st7   
    fcmovne        st7   
    fcmovnbe       st7   
    fcmovnu        st7   
    fucomi         st7   
    fcomi          st7   

    fadd           qword [rbx]     
    fmul           qword [rbx]     
    fcom           qword [rbx]     
    fcomp          qword [rbx]     
    fsub           qword [rbx]     
    fsubr          qword [rbx]     
    fdiv           qword [rbx]     
    fdivr          qword [rbx]     
    fadd           st7   
    fmul           st7   
    fcom           st7   
    fcomp          st7   
    fsubr          st7   
    fsub           st7   
    fdivr          st7   
    fdiv           st7   

    fld            qword [rbx]     
    fisttp         qword [rbx]      
    fst            qword [rbx]     
    fstp           qword [rbx]     
    frstor         [rbx]           
    fnsave         [rbx]           
    fnstsw         [rbx]            
    ffree          st7         
    fxch           st7   
    fst            st7   
    fstp           st7   
    fucom          st7   
    fucomp         st7   

    fiadd          dword [rbx]      
    fimul          dword [rbx]      
    ficom          dword [rbx]      
    ficomp         dword [rbx]      
    fisub          dword [rbx]      
    fisubr         dword [rbx]      
    fidiv          dword [rbx]      
    fidivr         dword [rbx]      

    fcompp                    
    faddp          st7   
    fmulp          st7   
    fcomp          st7   
    fsubrp         st7   
    fsubp          st7   
    fdivrp         st7   
    fdivp          st7   

    fild           qword [rbx]      
    fisttp         qword [rbx]      
    fist           dword [rbx]      
    fistp          qword [rbx]      
    fbld           [rbx]     
    fild           qword [rbx]      
    fbstp          [rbx]     
    fistp          qword [rbx]      

    ffreep         st7         
    fxch           st7   
    fstp           st7   
    fstp           st7   
    fnstsw         ax            
    fucomip        st7   
    fcomip         st7   
