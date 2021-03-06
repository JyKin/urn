#include <stdbool.h>
#include "urn-component.h"

typedef struct _UrnRemTimeSave {
    UrnComponent base;
    GtkWidget *container;
    GtkWidget *time_save_label;
    GtkWidget *time_save;
} UrnTimeSave;
extern UrnComponentOps urn_rem_time_save_operations;

#define REM_TIME_SAVE "Remaining time save"

UrnComponent *urn_component_rem_time_save_new() {
    UrnTimeSave *self;

    self = malloc(sizeof(UrnTimeSave));
    if (!self) return NULL;
    self->base.ops = &urn_rem_time_save_operations;

    self->container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    add_class(self->container, "footer");
    gtk_widget_show(self->container);

    self->time_save_label = gtk_label_new(REM_TIME_SAVE);
    add_class(self->time_save_label, "rem-time-save-label");
    gtk_widget_set_halign(self->time_save_label,
                          GTK_ALIGN_START);
    gtk_widget_set_hexpand(self->time_save_label, TRUE);
    gtk_container_add(GTK_CONTAINER(self->container),
            self->time_save_label);
    gtk_widget_show(self->time_save_label);

    self->time_save = gtk_label_new(NULL);
    add_class(self->time_save, "rem-time-save");
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
                       REM_TIME_SAVE);
    gtk_label_set_text(GTK_LABEL(self->time_save), "-");
}

static void time_save_draw(UrnComponent *self_, urn_game *game,
        urn_timer *timer) {
    UrnTimeSave *self = (UrnTimeSave *)self_;
    const char *label;
    char str[256];
    int prev, curr = timer->curr_split;
    if (curr == game->split_count || !game->segment_times[curr] || !game->best_segments[curr]) {
        gtk_label_set_text(GTK_LABEL(self->time_save), "-");
        return;
    }
    long long rem = 0;
    int start = curr;
    if (timer->started)
        start++;

    // when we lack segments times but have best_segments, we can compute the time to save by cumulating multiple splits
    bool accumulating = false;
    long long start_accu = 0;
    long long best_accu = 0;
    for (int i = start; i <game->split_count; i++) {
        if (accumulating) {
            // If we have neither a segment time nor a best segment, we're screwed
            // to do accumulation. So add what we know for sure when we tried to
            // accumulate.
            if (!game->best_segments[i] && !game->segment_times[i]) {
                accumulating = false;
                continue;
            }
            // If have a split time, we can stop accumulation.
            if (game->split_times[i]) {
                long long duration = game->split_times[i] - start_accu;
                long long local_best = game->best_segments[i] ? game->best_segments[i] : game->segment_times[i];
                rem += duration - best_accu - local_best;
                accumulating = false;
                start_accu = 0;
                best_accu = 0;
            } else {
                // otherwise, accumulate best duration
                best_accu  += game->best_segments[i] ? game->best_segments[i] : game->segment_times[i];
            }
        } else {
            // no best segment, ignore
            if (!game->best_segments[i]) {
                continue;
            }
            if (!game->segment_times[i]) {
                // no previous split times, ignore too, we can't start
                // accumulating
                if (i > 0 && !game->split_times[i - 1]) {
                    continue;
                }
                accumulating = true;
                best_accu  = game->best_segments[i];
                start_accu = i > 0 ? game->split_times[i-1] : 0;
                continue;
            }
            rem += game->segment_times[i] - game->best_segments[i];
        }
    }


    urn_delta_string(str, rem);
    gtk_label_set_text(GTK_LABEL(self->time_save), str+1);
}

UrnComponentOps urn_rem_time_save_operations = {
    .delete = time_save_delete,
    .widget = time_save_widget,
    .show_game = time_save_show_game,
    .clear_game = time_save_clear_game,
    .draw = time_save_draw
};
