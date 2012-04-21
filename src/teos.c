// R CMD SHLIB dlsym2.c
// R --no-save < dlsym.R
#include <dlfcn.h>
#include <R.h>
#include <Rdefines.h>
#include <Rinternals.h>
static int first_teos_call = 1;
static void *teos_handle = NULL;
void gsw3a(char **lib, char **name, int *n, double *a1, double *a2, double *a3, double *rval)
{
    if (first_teos_call) {
        teos_handle = dlopen(*lib, RTLD_LOCAL|RTLD_LAZY);
        first_teos_call = 0;
    }
    if (!teos_handle)
        error("cannot open TEOS library %s; error is: %s", *lib, dlerror());
    //Rprintf("%s:%d about to do try to find dlsym(handle, \"%s\"\n", __FILE__, __LINE__, *name);
    double (*f3)(double, double, double) = dlsym(teos_handle, *name);
    if (!f3) 
        error("cannot find \"%s\" in TEOS library %s; error is: %s", *name, *lib, dlerror());
    //Rprintf("%s:%d about to do the loop calling f3 (\"%s\")\n", __FILE__, __LINE__, *name);
    for (int i = 0; i < *n; i++) {
        //Rprintf("%s:%d in loop i=%d, a1[i]=%f, a2[i]=%f a3[i]=%f\n",__FILE__,__LINE__,i,a1[i],a2[i],a3[i]);
        rval[i] = (*f3)(a1[i], a2[i], a3[i]);
    }
}

void gsw4a(char **lib, char **name, int *n, double *a1, double *a2, double *a3, double *a4, double *rval)
{
    //*lib = "/usr/local/lib/libgswteos-10.so"; // FIXME: testing, since getting an error otherwise
    //Rprintf("DEBUG: using library \"%s\"\n", *lib);
    void *handle = dlopen(*lib, RTLD_LOCAL|RTLD_LAZY);
    if (!handle) {
        error("cannot open TEOS library %s; error is: %s", *lib, dlerror());
    }
    double (*f4)(double, double, double, double) = dlsym(handle, *name);
    if (!f4) 
        error("cannot find \"%s\" in TEOS library %s", *name, *lib);
    for (int i = 0; i < *n; i++) {
        rval[i] = (*f4)(a1[i], a2[i], a3[i], a4[i]);
    }
    dlclose(handle);
}

