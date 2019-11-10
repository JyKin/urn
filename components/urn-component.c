#include "urn-component.h"

UrnComponent *urn_component_title_new();
UrnComponent *urn_component_splits_new();
UrnComponent *urn_component_timer_new();
UrnComponent *urn_component_time_save_new();
UrnComponent *urn_component_rem_time_save_new();
UrnComponent *urn_component_current_pace_new();
UrnComponent *urn_component_prev_segment_new();
UrnComponent *urn_component_best_sum_new();
UrnComponent *urn_component_pb_new();
UrnComponent *urn_component_wr_new();

UrnComponentAvailable urn_components[] = {
    {"title",        urn_component_title_new},
    {"splits",       urn_component_splits_new},
    {"timer",        urn_component_timer_new},
    {"time-save", urn_component_time_save_new},
    {"rem-time-save", urn_component_rem_time_save_new},
    {"current-pace", urn_component_current_pace_new},
    {"prev-segment", urn_component_prev_segment_new},
    {"best-sum",     urn_component_best_sum_new},
    {"pb",           urn_component_pb_new},
    {"wr",           urn_component_wr_new},
    {NULL, NULL}
};

void add_class(GtkWidget *widget, const char *class) {
    gtk_style_context_add_class(gtk_widget_get_style_context(widget), class);
}

void remove_class(GtkWidget *widget, const char *class) {
    gtk_style_context_remove_class(gtk_widget_get_style_context(widget), class);
}
