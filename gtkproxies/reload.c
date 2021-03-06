/* 
 * Copyright 2011 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy 
 * defined in Section 14 of version 3 of the license.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * #include <gtk/gtk.h>
 */

#include <gdk/gdkx.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>

void printHelp()
{
    printf(
        "Small utility for sending signal to reload theme in Gtk programs.\n"
        "\n"
        "Using:\n"
        "reload_gtk_apps <args>\n"
        "\n"
        "Arguments:\n"
        "  -h, --help\tShows this help\n"
        "  -V, --version\tPrints the program version\n"
    );
}
GdkEventClient createEvent()
{
    GdkEventClient event;
    event.type = GDK_CLIENT_EVENT;
    event.send_event = TRUE;
    event.window = NULL;
    event.message_type = gdk_atom_intern("_GTK_READ_RCFILES", FALSE);
    event.data_format = 8;
    
    return event;
}

int main(int argc, char** argv)
{
    gtk_init(&argc, &argv);
    int i;
    for(i=0; i<argc; i++) {
        if(strcmp("-h", argv[i])==0 || strcmp("--help", argv[i])==0) {
            printHelp();
            return 0;
        }
        else if(strcmp("-V", argv[i])==0 || strcmp("--version", argv[i])==0) {
            printf("reload_gtk_apps version 1.0\n");
            return 0;
        }
    }
    
    GdkEventClient event = createEvent();
    gdk_event_send_clientmessage_toall((GdkEvent *)&event);
    return 0;
}
