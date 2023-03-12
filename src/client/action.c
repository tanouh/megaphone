#include "view.h"

#include <stdio.h>
#include <stdlib.h>

int post()
{
	int num_thread = choose_thread_to_post_in();
	if (num_thread == 0)
		print_error("Réponse invalide.\n");
	return 1;
	char *text = post_ticket(num_thread);
	printf("%s\n", text);
	// TODO : Poster billet

	return 0;
}

int add_file()
{
	char *file_link = add_file_to_thread();
	printf("%s\n", file_link);

	// TODO : Ajouter fichier
	return 0;
}

int see_old_ticket()
{
	int num_thread = select_thread_to_unfold();
	if (num_thread == 0) {
		print_error("Numéro de thread incorrect.\n");
		return 1;
	}
	int nb_ticket = see_previous_tickets(num_thread);
	if (nb_ticket == 0) {
		print_error("Nombre de billets à voir invalide.\n");
		return 1;
	}
	printf("%d\n", nb_ticket);

	// TODO : Voir anciens billets

	return 0;
}

int download()
{
	int num_file = download_file();
	if (num_file == 0) {
		print_error("Numéro du fichier à télécharger invalide.\n");
		return 1;
	}
	printf("%d\n", num_file);

	// TODO : Télécharger fichiers

	return 0;
}

int subscribe()
{
	int num_thread = subscribe_to_thread();
	if (num_thread == 0) {
		print_error("Numéro du fil auquel s'abonner.\n");
		return 1;
	}
	printf("%d\n", num_thread);

	// TODO : S'abonner à un fil

	return 0;
}