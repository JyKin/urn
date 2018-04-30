#include "urn-component.h"

typedef struct _UrnTimeSave {
    UrnComponent base;
    GtkWidget *container;
    GtkWidget *time_save_label;
    GtkWidget *time_save;
} UrnTimeSave;
extern UrnComponentOps urn_time_save_operations;

#define TIME_SAVE "possible time save"

UrnComponent *urn_component_time_save_new() {
    UrnTimeSave *self;

    self = malloc(sizeof(UrnTimeSave));
    if (!self) return NULL;
    self->base.ops = &urn_time_save_operations;

    self->container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    add_class(self->container, "footer");
    gtk_widget_show(self->container);

    self->time_save_label = gtk_label_new(TIME_SAVE);
    add_class(self->time_save_label, "time-save-label");
    gtk_widget_set_halign(self->time_save_label,
                          GTK_ALIGN_START);
    gtk_widget_set_hexpand(self->time_save_label, TRUE);
    gtk_container_add(GTK_CONTAINER(self->container),
            self->time_save_label);
    gtk_widget_show(self->time_save_label);

    self->time_save = gtk_label_new(NULL);
    add_class(self->time_save, "time-save");
    gtk_widget_set_halign(self->time_save, GTK_ALIGN_END);
    gtk_container_add(GTK_CONTAINER(self->container), self->time_save);
    gtk_widget_show(self->time_save);

    return (UrnComponent *)self;
}

static void time_save_delete(UrnComponent *self) {
    free(self);
}

static GtkWidget *time_save_widget(UrnComponent *self) {
    return ((UrnTimeSave *)self)->container;
}

static void time_save_show_game(UrnComponent *self_,
        urn_game *game, urn_timer *timer) {
}

static void time_save_clear_game(UrnComponent *self_) {
    UrnTimeSave *self = (UrnTimeSave *)self_;
    gtk_label_set_text(GTK_LABEL(self->time_save_label),
                       TIME_SAVE);
    gtk_label_set_text(GTK_LABEL(self->time_save), "-");
}

static void time_save_draw(UrnComponent *self_, urn_game *game,
        urn_timer *timer) {
    UrnTimeSave *self = (UrnTimeSave *)self_;
    const char *label;
    char str[256];
    int prev, curr = timer->curr_split;
    if (!timer->started || curr == game->split_count || !game->segment_times[curr] || !game->best_segments[curr]) {
        gtk_label_set_text(GTK_LABEL(self->time_save), "-");
        return;
    }

    urn_delta_string(str, game->segment_times[curr] - game->best_segments[curr]);
    gtk_label_set_text(GTK_LABEL(self->time_save), str+1);
}

UrnComponentOps urn_time_save_operations = {
    .delete = time_save_delete,
    .widget = time_save_widget,
    .show_game = time_save_show_game,
    .clear_game = time_save_clear_game,
    .draw = time_save_draw
};
