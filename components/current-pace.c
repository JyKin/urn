#include <stdbool.h>
#include "urn-component.h"

typedef struct _UrnCurrentPace {
    UrnComponent base;
    GtkWidget *container;
    GtkWidget *current_pace_label;
    GtkWidget *current_pace;
} UrnCurrentPace;
extern UrnComponentOps urn_current_pace_operations;

#define CURRENT_PACE "Best possible time"

UrnComponent *urn_component_current_pace_new() {
    UrnCurrentPace *self;

    self = malloc(sizeof(UrnCurrentPace));
    if (!self) return NULL;
    self->base.ops = &urn_current_pace_operations;

    self->container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    add_class(self->container, "footer");
    gtk_widget_show(self->container);

    self->current_pace_label = gtk_label_new(CURRENT_PACE);
    add_class(self->current_pace_label, "current-pace-label");
    gtk_widget_set_halign(self->current_pace_label,
                          GTK_ALIGN_START);
    gtk_widget_set_hexpand(self->current_pace_label, TRUE);
    gtk_container_add(GTK_CONTAINER(self->container),
            self->current_pace_label);
    gtk_widget_show(self->current_pace_label);

    self->current_pace = gtk_label_new(NULL);
    add_class(self->current_pace, "current-pace");
    gtk_widget_set_halign(self->current_pace, GTK_ALIGN_END);
    gtk_container_add(GTK_CONTAINER(self->container), self->current_pace);
    gtk_widget_show(self->current_pace);

    return (UrnComponent *)self;
}

static void current_pace_delete(UrnComponent *self) {
    free(self);
}

static GtkWidget *current_pace_widget(UrnComponent *self) {
    return ((UrnCurrentPace *)self)->container;
}

static void current_pace_show_game(UrnComponent *self_,
        urn_game *game, urn_timer *timer) {
}

static void current_pace_clear_game(UrnComponent *self_) {
    UrnCurrentPace *self = (UrnCurrentPace *)self_;
    gtk_label_set_text(GTK_LABEL(self->current_pace_label),
                       CURRENT_PACE);
    gtk_label_set_text(GTK_LABEL(self->current_pace), "-");
}

static void current_pace_draw(UrnComponent *self_, urn_game *game,
        urn_timer *timer) {
    UrnCurrentPace *self = (UrnCurrentPace *)self_;
    const char *label;
    char str[256];
    int prev, curr = timer->curr_split;
    if (curr == game->split_count || !game->segment_times[curr] || !game->best_segments[curr]) {
        gtk_label_set_text(GTK_LABEL(self->current_pace), "-");
        return;
    }
    long long lost = 0;
    for (int i = 0; i < game->split_count ; i++) {
        if (i < curr) {
            if (timer->segment_times[i]) {
                lost += timer->segment_times[i];
            } else {
                lost += game->best_segments[i];
            }
        } else {
            lost += game->best_segments[i];
        }
    }


    urn_split_string(str, lost);
    gtk_label_set_text(GTK_LABEL(self->current_pace), str);
}

UrnComponentOps urn_current_pace_operations = {
    .delete = current_pace_delete,
    .widget = current_pace_widget,
    .show_game = current_pace_show_game,
    .clear_game = current_pace_clear_game,
    .draw = current_pace_draw
};
