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
    char str[256];
    int curr = timer->curr_split;
    if (curr == game->split_count) {
        gtk_label_set_text(GTK_LABEL(self->current_pace), "-");
        return;
    }
    long long bests_next = 0;
    for (int i = curr+1; i < game->split_count; i++) {
        if (!game->best_segments[i]) {
            gtk_label_set_text(GTK_LABEL(self->current_pace), "-");
            return;
        }
        bests_next += game->best_segments[i];
    }
    if (curr == 0 || timer->split_times[curr-1] == 0 || game->best_segments[curr] == 0
            || timer->segment_times[curr] > game->best_segments[curr]) {
        bests_next += timer->now;
    } else {
        bests_next += timer->split_times[curr-1];
    }

    urn_split_string(str, bests_next);
    gtk_label_set_text(GTK_LABEL(self->current_pace), str);
}

UrnComponentOps urn_current_pace_operations = {
    .delete = current_pace_delete,
    .widget = current_pace_widget,
    .show_game = current_pace_show_game,
    .clear_game = current_pace_clear_game,
    .draw = current_pace_draw
};
