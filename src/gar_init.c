/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <R.h>
#include <Rinternals.h>
#include <stdlib.h> // for NULL
#include <R_ext/Rdynload.h>

/* .Call calls */
extern SEXP gar_add_aoi(SEXP, SEXP);
extern SEXP gar_add_aoi_rectangle(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP gar_create(SEXP);
extern SEXP gar_create_aoi(SEXP, SEXP);
extern SEXP gar_get_filter_parameter(SEXP);
extern SEXP gar_get_filter_parameter_default();
extern SEXP gar_init();
extern SEXP gar_parse(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP gar_set_screen(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);

static const R_CallMethodDef CallEntries[] = {
    {"gar_add_aoi",                      (DL_FUNC) &gar_add_aoi,                       2},
    {"gar_add_aoi_rectangle",            (DL_FUNC) &gar_add_aoi_rectangle,             6},
    {"gar_create",                       (DL_FUNC) &gar_create,                        1},
    {"gar_create_aoi",                   (DL_FUNC) &gar_create_aoi,                    2},
    {"gar_get_filter_parameter",         (DL_FUNC) &gar_get_filter_parameter,          1},
    {"gar_get_filter_parameter_default", (DL_FUNC) &gar_get_filter_parameter_default,  0},
    {"gar_init",                         (DL_FUNC) &gar_init,                          0},
    {"gar_parse",                        (DL_FUNC) &gar_parse,                        12},
    {"gar_set_screen",                   (DL_FUNC) &gar_set_screen,                   10},
    {NULL, NULL, 0}
};

void R_init_gar(DllInfo *dll)
{
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
