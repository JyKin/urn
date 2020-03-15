#include <stdbool.h>
#include "urn-component.h"

typedef struct _UrnBestPossible {
    UrnComponent base;
    GtkWidget *container;
    GtkWidget *best_possible_label;
    GtkWidget *best_possible;
} UrnBestPossible;
extern UrnComponentOps urn_best_possible_operations;

#define BEST_POSSIBLE "Best possible time"

UrnComponent *urn_component_best_possible_new() {
    UrnBestPossible *self;

    self = malloc(sizeof(UrnBestPossible));
    if (!self) return NULL;
    self->base.ops = &urn_best_possible_operations;

    self->container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    add_class(self->container, "footer");
    gtk_widget_show(self->container);

    self->best_possible_label = gtk_label_new(BEST_POSSIBLE);
    add_class(self->best_possible_label, "best-possible-label");
    gtk_widget_set_halign(self->best_possible_label,
                          GTK_ALIGN_START);
    gtk_widget_set_hexpand(self->best_possible_label, TRUE);
    gtk_container_add(GTK_CONTAINER(self->container),
            self->best_possible_label);
    gtk_widget_show(self->best_possible_label);

    self->best_possible = gtk_label_new(NULL);
    add_class(self->best_possible, "best-possible");
    gtk_widget_set_halign(self->best_possible, GTK_ALIGN_END);
    gtk_container_add(GTK_CONTAINER(self->container), self->best_possible);
    gtk_widget_show(self->best_possible);

    return (UrnComponent *)self;
}

static void best_possible_delete(UrnComponent *self) {
    free(self);
}

static GtkWidget *best_possible_widget(UrnComponent *self) {
    return ((UrnBestPossible *)self)->container;
}

static void best_possible_show_game(UrnComponent *self_,
        urn_game *game, urn_timer *timer) {
}

static void best_possible_clear_game(UrnComponent *self_) {
    UrnBestPossible *self = (UrnBestPossible *)self_;
    gtk_label_set_text(GTK_LABEL(self->best_possible_label),
                       BEST_POSSIBLE);
    gtk_label_set_text(GTK_LABEL(self->best_possible), "-");
}

static void best_possible_draw(UrnComponent *self_, urn_game *game,
        urn_timer *timer) {
    UrnBestPossible *self = (UrnBestPossible *)self_;
    char str[256];
    int curr = timer->curr_split;
    if (curr == game->split_count || !timer->started) {
        gtk_label_set_text(GTK_LABEL(self->best_possible), "-");
        return;
    }
    long long bests_next = 0;
    for (int i = curr+1; i < game->split_count; i++) {
        if (!game->best_segments[i]) {
            goto fail;
        }
        bests_next += game->best_segments[i];
    }
    if (curr == 0 || timer->split_times[curr-1]) {
        if (timer->segment_times[curr] > game->best_segments[curr]) {
            bests_next += timer->time;
        } else {
            bests_next += game->best_segments[curr];
            if (curr)
                bests_next += timer->split_times[curr-1];
        }
    } else {
        int baseidx = curr - 1;
        while (baseidx >= 0 && timer->split_times[baseidx] == 0)
            baseidx--;
        long long base = (baseidx < 0) ? 0 : timer->split_times[baseidx];
        long long best = 0;
        for (int i = baseidx+1; i <= curr; i++) {
            if (!game->best_segments[i]) {
                goto fail;
            }
            best += game->best_segments[i];
        }
        long long seg = timer->time - base;
        if (seg > best) {
            bests_next += timer->time;
        } else {
            bests_next += best;
            bests_next += base;
        }
    }


    urn_split_string(str, bests_next);
    gtk_label_set_text(GTK_LABEL(self->best_possible), str);
    return;
fail:
    gtk_label_set_text(GTK_LABEL(self->best_possible), "-");
    return;
}

UrnComponentOps urn_best_possible_operations = {
    .delete = best_possible_delete,
    .widget = best_possible_widget,
    .show_game = best_possible_show_game,
    .clear_game = best_possible_clear_game,
    .draw = best_possible_draw
};
