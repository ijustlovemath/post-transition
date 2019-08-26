#include <stdio.h>
#include <stdlib.h>
#define MAX_STRING_LENGTH 6

struct package
{
	char* id;
	int weight;
};

typedef struct package package;

struct post_office
{
	int min_weight;
	int max_weight;
	package* packages;
	int packages_count;
};

typedef struct post_office post_office;

struct town
{
	char* name;
	post_office* offices;
	int offices_count;
};

typedef struct town town;



void print_all_packages(town t) {
    printf("%s:\n", t.name);
    for(int i = 0; i < t.offices_count; ++i) {
        printf("\t%d:\n", i);
        for(int j = 0; j < t.offices[i].packages_count; ++j) {
            printf("\t\t:%s\n", t.offices[i].packages[j].id);
        }
    }
}

#include<string.h>

int package_eligible(const package *package, const post_office *target)
{
    return ((package->weight <= target->max_weight) 
                && (package->weight >= target->min_weight)
            );
}

void move_package_to_target(const package *package, post_office *target)
{
    /* increase package count */
    target->packages_count++;

    /* allocate more space for the new package */
    target->packages = realloc(target->packages, sizeof(package) * target->packages_count);

    /* add package to end of queue */
    target->packages[target->packages_count - 1] = *package;
}

void shift_source_packages_up(post_office *source, int i)
{
    /* readjust array */
    for(; i < source->packages_count - 1; ++i) {
        source->packages[i] = source->packages[i + 1];
    }
    /* decrease package count */
    source->packages_count--;

    /* deallocate space */
    source->packages = realloc(source->packages, sizeof(package) * source->packages_count);
}

void send_all_acceptable_packages(town* source, int source_office_index, town* target, int target_office_index) {
    /* 1. Collect list of eligible packages */
    /* 1a. As we find eligible packages, delete it from source office, add it to dest office */
    /* 2. Process in detail:
    *      - identify eligible package index, save a pointer to it
    *      - move it to the end of the queue in the destination office
    *      - increase destination package count
    *      - shift all packages with a larger source index up the queue (up to source package count)
    *      - decrease source package count
    */	
    post_office *source_office, *target_office;

    source_office = &source->offices[source_office_index];
    target_office = &target->offices[target_office_index];

    package *current = NULL;

    int packages_left = source_office->packages_count > 0;
    int i = 0;
    while(packages_left) {
        for(; i < source_office->packages_count; ++i) {
            current = &source_office->packages[i];
            if(package_eligible(current, target_office)) {
                move_package_to_target(current, target_office);
                shift_source_packages_up(source_office, i);
                packages_left = i < source_office->packages_count;
                break;
            }
        }
    }
}

town town_with_most_packages(town* towns, int towns_count) {
    town t, it;
    int most_packages = 0;
    for(int i = 0; i < towns_count; ++i) {
        it = towns[i];
        int total_packages = 0;
        for(int j = 0; j < it.offices_count; ++j) {
            total_packages += it.offices[j].packages_count;
        }
        if(total_packages > most_packages) {
            t = it;
            most_packages = total_packages;
        }

    }
    return t;
}

town* find_town(town* towns, int towns_count, char* name) {
    for(int i = 0; i < towns_count; ++i) {
        if(!strcmp(towns[i].name, name)) {
            return &towns[i];
        }
    }
    return NULL;
}

int main()
{
	int towns_count;
	scanf("%d", &towns_count);
	town* towns = malloc(sizeof(town)*towns_count);
	for (int i = 0; i < towns_count; i++) {
		towns[i].name = malloc(sizeof(char) * MAX_STRING_LENGTH);
		scanf("%s", towns[i].name);
		scanf("%d", &towns[i].offices_count);
		towns[i].offices = malloc(sizeof(post_office)*towns[i].offices_count);
		for (int j = 0; j < towns[i].offices_count; j++) {
			scanf("%d%d%d", &towns[i].offices[j].packages_count, &towns[i].offices[j].min_weight, &towns[i].offices[j].max_weight);
			towns[i].offices[j].packages = malloc(sizeof(package)*towns[i].offices[j].packages_count);
			for (int k = 0; k < towns[i].offices[j].packages_count; k++) {
				towns[i].offices[j].packages[k].id = malloc(sizeof(char) * MAX_STRING_LENGTH);
				scanf("%s", towns[i].offices[j].packages[k].id);
				scanf("%d", &towns[i].offices[j].packages[k].weight);
			}
		}
	}
	int queries;
	scanf("%d", &queries);
	char town_name[MAX_STRING_LENGTH];
	while (queries--) {
		int type;
		scanf("%d", &type);
		switch (type) {
		case 1:
			scanf("%s", town_name);
			town* t = find_town(towns, towns_count, town_name);
			print_all_packages(*t);
			break;
		case 2:
			scanf("%s", town_name);
			town* source = find_town(towns, towns_count, town_name);
			int source_index;
			scanf("%d", &source_index);
			scanf("%s", town_name);
			town* target = find_town(towns, towns_count, town_name);
			int target_index;
			scanf("%d", &target_index);
			send_all_acceptable_packages(source, source_index, target, target_index);
			break;
		case 3:
			printf("Town with the most number of packages is %s\n", town_with_most_packages(towns, towns_count).name);
			break;
		}
	}
	return 0;
}


