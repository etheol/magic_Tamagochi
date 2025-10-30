#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <windows.h>
#include <locale.h>

// Типы питомцев
typedef enum {
    CHIMERA,
    DRAGON,
    UNICORN,
    AXOLOTL,
    PHOENIX
} PetType;

// Эволюционные стадии
typedef enum {
    EGG,
    BABY,
    TEEN,
    ADULT,
    MYTHICAL,
    LEGENDARY
} EvolutionStage;

// Погода
typedef enum {
    SUNNY,
    RAINY,
    STORMY,
    MAGICAL
} Weather;

// Время суток
typedef enum {
    MORNING,
    DAY,
    EVENING,
    NIGHT
} TimeOfDay;

// Достижения
typedef struct {
    int first_evolution;
    int played_games;
    int survived_storms;
    int reached_mythical;
    int reached_legendary;
} Achievements;

// Структура питомца
typedef struct {
    char name[20];
    PetType type;
    EvolutionStage stage;
    int hunger;
    int happiness;
    int energy;
    int magic;       
    int age;
    int is_alive;
    int evolution_points;
    int legendary_points;
} Pet;

// Глобальные переменные игры
Weather current_weather;
TimeOfDay current_time;
Achievements achievements;
int game_cycles = 0;
int tokens = 10; // Жетоны для замены питомца

// Названия и внешний вид для каждого типа и стадии
const char* type_names[] = {"Химера", "Дракон", "Единорог", "Аксолотль", "Феникс"};
const char* stage_names[] = {"Яйцо", "Детёныш", "Подросток", "Взрослый", "Мифический", "ЛЕГЕНДАРНЫЙ"};

// Функция для рандомного выбора питомца с разными вероятностями
PetType get_random_pet_type() 
{
    int chance = rand() % 100; // Случайное число от 0 до 99
    
    if (chance < 40) { // 40% вероятность
        return CHIMERA;
    } else if (chance < 80) { // 40% вероятность (40+40=80)
        return AXOLOTL;
    } else if (chance < 88) { // 8% вероятность (80+8=88)
        return DRAGON;
    } else if (chance < 97) { // 9% вероятность (88+9=97)
        return PHOENIX;
    } else { // 3% вероятность (97+3=100)
        return UNICORN;
    }
}
// Функции для мини-игр
int magic_guess_game() {
    printf("\n🎮 Магическая угадайка!\n");
    printf("Я загадал число от 1 до 5. Угадай!\n");
    
    int secret = rand() % 5 + 1;
    int guess, attempts = 3;
    
    while(attempts > 0) {
        printf("Попыток: %d. Твой выбор: ", attempts);
        scanf("%d", &guess);
        
        if(guess == secret) {
            printf("✨ Правильно! Магическая победа!\n");
            return 1;
        } else {
            printf("❌ Не угадал!\n");
            attempts--;
        }
    }
    printf("💫 Заклинание рассеялось...\n");
    return 0;
}

int element_battle() {
    printf("\n⚔️  Битва стихий!\n");
    printf("Выбери стихию: 1-Огонь 🔥, 2-Вода 💧, 3-Земля 🌿, 4-Воздух 🌪\n");
    
    int player_choice, enemy_choice = rand() % 4 + 1;
    scanf("%d", &player_choice);
    
    const char* elements[] = {"", "Огонь", "Вода", "Земля", "Воздух"};
    printf("Ты выбрал: %s vs ", elements[player_choice]);
    printf("Противник: %s\n", elements[enemy_choice]);
    
    // Простая логика победы (камень-ножницы-бумага)
    if(player_choice == enemy_choice) {
        printf("⚡ Ничья! Магический резонанс!\n");
        return 1;
    } else if((player_choice == 1 && enemy_choice == 3) || // Огонь побеждает землю
              (player_choice == 2 && enemy_choice == 1) || // Вода побеждает огонь
              (player_choice == 3 && enemy_choice == 4) || // Земля побеждает воздух
              (player_choice == 4 && enemy_choice == 2)) { // Воздух побеждает воду
        printf("🎇 Победа! Твоя стихия сильнее!\n");
        return 2;
    } else {
        printf("💥 Поражение! Противник сильнее!\n");
        return 0;
    }
}

// Новая мини-игра для легендарной стадии
int legendary_ritual() {
    printf("\n🔮 ВЕЛИКИЙ РИТУАЛ ЛЕГЕНДЫ!\n");
    printf("Собери магическую последовательность из 3 символов!\n");
    printf("Символы: 🔥 🌊 🌪 🌑 🌟\n");
    
    char sequence[] = {'1', '2', '3'}; // Простая последовательность
    char player_seq[4];
    
    printf("Введи 3 цифры (1-5): ");
    scanf("%s", player_seq);
    
    int correct = 1;
    for(int i = 0; i < 3; i++) {
        if(player_seq[i] != sequence[i]) {
            correct = 0;
            break;
        }
    }
    
    if(correct) {
        printf("🌈 ВЕЛИКИЙ РИТУАЛ УСПЕШЕН! Сила легенды пробуждена!\n");
        return 3;
    } else {
        printf("💫 Ритуал не удался... но ты чувствуешь растущую силу!\n");
        return 1;
    }
}

// Функции эволюции
void evolve_pet(Pet *pet) {
    if(pet->stage < MYTHICAL && pet->evolution_points >= 30) {
        pet->stage++;
        pet->evolution_points = 0;
        printf("\n🌟 ЭВОЛЮЦИЯ! %s превращается в %s!\n", 
               stage_names[pet->stage-1], stage_names[pet->stage]);
        
        if(pet->stage == BABY) achievements.first_evolution = 1;
        if(pet->stage == MYTHICAL) achievements.reached_mythical = 1;
    }
    else if(pet->stage == MYTHICAL && pet->legendary_points >= 50) {
        pet->stage = LEGENDARY;
        printf("\n💥⚡🌟 ЛЕГЕНДАРНАЯ ЭВОЛЮЦИЯ! %s становится %s!\n", 
               pet->name, stage_names[LEGENDARY]);
        achievements.reached_legendary = 1;
        
        // Бонусы за легендарную стадию
        pet->magic = 100;
        pet->happiness = 100;
        pet->energy = 100;
    }
}

// Визуализация питомца с легендарной стадией
void draw_pet_art(Pet *pet) {
    printf("\n");
    switch(pet->type) {
        case CHIMERA:
            if(pet->stage == EGG) printf("   🥚 Магическое яйцо шевелится...\n");
            else if(pet->stage == BABY) printf("   🦁🐐🐍 Маленькая Химера пищит\n");
            else if(pet->stage == TEEN) printf("   🦁🐐🐍 Химера рычит тремя головами\n");
            else if(pet->stage == ADULT) printf("   🦁🐐🐍🔥 ВЗРОСЛАЯ ХИМЕРА ИЗВЕРГАЕТ ПЛАМЯ!\n");
            else if(pet->stage == MYTHICAL) printf("   🦁🐐🐍💫 МИФИЧЕСКАЯ ХИМЕРА - ЛЕГЕНДА ЖИВЁТ!\n");
            else printf("   🦁👑🐐🌟🐍💥 ЛЕГЕНДАРНАЯ ХИМЕРА - ВОПЛОЩЕНИЕ ХАОСА!\n");
            break;
        case DRAGON:
            if(pet->stage == EGG) printf("   🥚 Чешуйчатое яйцо светится\n");
            else if(pet->stage == BABY) printf("   🐲 Дракончик чихает дымком\n");
            else if(pet->stage == TEEN) printf("   🐲🔥 Подросток-дракон тренирует пламя\n");
            else if(pet->stage == ADULT) printf("   🐲🔥💨 ВЗРОСЛЫЙ ДРАКОН ЛЕТАЕТ В ОБЛАКАХ!\n");
            else if(pet->stage == MYTHICAL) printf("   🐲🔥🌟 МИФИЧЕСКИЙ ДРАКОН - ПОВЕЛИТЕЛЬ СТИХИЙ!\n");
            else printf("   🐲👑🔥💥🌟 ЛЕГЕНДАРНЫЙ ДРАКОН - СОЗДАТЕЛЬ МИРОВ!\n");
            break;
        case UNICORN:
            if(pet->stage == EGG) printf("   🥚 Радужное яйцо переливается\n");
            else if(pet->stage == BABY) printf("   🦄🍭 Единорожек оставляет радужные следы\n");
            else if(pet->stage == TEEN) printf("   🦄🌈 Подросток-единорог учится магии\n");
            else if(pet->stage == ADULT) printf("   🦄🌈✨ ВЗРОСЛЫЙ ЕДИНОРОГ ИСЦЕЛЯЕТ СЕРДЦА!\n");
            else if(pet->stage == MYTHICAL) printf("   🦄🌈💫 МИФИЧЕСКИЙ ЕДИНОРОГ - ХРАНИТЕЛЬ МЕЧТ!\n");
            else printf("   🦄👑🌈🌟💫 ЛЕГЕНДАРНЫЙ ЕДИНОРОГ - ИСТОЧНИК ВСЕХ ЧУДЕС!\n");
            break;
        case AXOLOTL:
            if(pet->stage == EGG) printf("   🥚 Водное яйцо пузырится\n");
            else if(pet->stage == BABY) printf("   🦎🌊 Аксолотлик улыбается\n");
            else if(pet->stage == TEEN) printf("   🦎💧 Подросток-аксолотль регенерирует\n");
            else if(pet->stage == ADULT) printf("   🦎💧🔮 ВЗРОСЛЫЙ АКСОЛОТЛЬ ВИДИТ БУДУЩЕЕ!\n");
            else if(pet->stage == MYTHICAL) printf("   🦎💧🌟 МИФИЧЕСКИЙ АКСОЛОТЛЬ - ВЕЧНЫЙ ДУХ ВОДЫ!\n");
            else printf("   🦎👑💧🌟🌊 ЛЕГЕНДАРНЫЙ АКСОЛОТЛЬ - ПЕРВОИСТОЧНИК ЖИЗНИ!\n");
            break;
        case PHOENIX:
            if(pet->stage == EGG) printf("   🥚 Огненное яйцо излучает тепло\n");
            else if(pet->stage == BABY) printf("   🐦🔥 Птенец феникса искрится\n");
            else if(pet->stage == TEEN) printf("   🐦🔥🌅 Подросток-феникс учится возрождаться\n");
            else if(pet->stage == ADULT) printf("   🐦🔥✨ ВЗРОСЛЫЙ ФЕНИКС ПЫЛАЕТ ВОССЛАВЛЕНИЕМ!\n");
            else if(pet->stage == MYTHICAL) printf("   🐦🔥💫 МИФИЧЕСКИЙ ФЕНИКС - ВЕЧНЫЙ ЦИКЛ ЖИЗНИ!\n");
            else printf("   🐦👑🔥🌟💥 ЛЕГЕНДАРНЫЙ ФЕНИКС - САМО ВОПЛОЩЕНИЕ ВОЗРОЖДЕНИЯ!\n");
            break;
    }
}

// Обновление погоды и времени
void update_environment() {
    game_cycles++;
    
    // Смена времени суток каждые 5 циклов
    current_time = (game_cycles / 5) % 4;
    
    // Случайная смена погоды
    if(rand() % 5 == 0) {
        current_weather = rand() % 4;
    }
}

// Влияние окружающей среды на питомца
void apply_environment_effects(Pet *pet) {
    const char* time_names[] = {"🌅 Утро", "🌞 День", "🌇 Вечер", "🌙 Ночь"};
    const char* weather_names[] = {"☀️  Солнечно", "🌧 Дождливо", "⛈ Гроза", "🌈 Магическая буря"};
    
    printf("\n%s | %s\n", time_names[current_time], weather_names[current_weather]);
    
    // Эффекты времени суток
    switch(current_time) {
        case MORNING:
            pet->energy += 5;
            break;
        case NIGHT:
            pet->energy -= 3;
            if(pet->type == UNICORN) pet->magic += 10; // Единороги ночью сильнее
            break;
    }
    
    // Эффекты погоды
    switch(current_weather) {
        case SUNNY:
            pet->happiness += 5;
            break;
        case RAINY:
            pet->happiness -= 5;
            if(pet->type == AXOLOTL) { // Аксолотли любят дождь
                pet->happiness += 15;
                pet->magic += 5;
            }
            break;
        case STORMY:
            pet->happiness -= 10;
            pet->energy -= 5;
            achievements.survived_storms++;
            break;
        case MAGICAL:
            pet->magic += 20;
            pet->happiness += 15;
            if(pet->stage >= MYTHICAL) {
                pet->legendary_points += 5; // Легендарные существа получают бонус
            }
            printf("💫 Магическая буря наполняет %s силой!\n", pet->name);
            break;
    }
    
    // Особые бонусы для легендарной стадии
    if(pet->stage == LEGENDARY) {
        pet->magic += 5; // Постоянная регенерация магии
        if(rand() % 3 == 0) {
            printf("👑 %s излучает ауру легенды, стабилизируя все показатели!\n", pet->name);
            pet->hunger = (pet->hunger > 50) ? pet->hunger - 10 : pet->hunger;
            pet->happiness = (pet->happiness < 80) ? pet->happiness + 10 : pet->happiness;
        }
    }
}

// Создание нового питомца
Pet create_new_pet() {
    Pet new_pet = {"", 0, EGG, 50, 70, 80, 30, 0, 1, 0, 0};
    new_pet.type = get_random_pet_type();
    return new_pet;
}

// Функция выбора питомца в начале игры
Pet choose_starting_pet() {
    Pet chosen_pet;
    int choosing = 1;
    
    while (choosing) {
        chosen_pet = create_new_pet();
        
        printf("\n🎰 Тебе выпал: %s!\n", type_names[chosen_pet.type]);
        printf("📊 Шанс выпадения: ");
        switch(chosen_pet.type) {
            case CHIMERA: printf("40%% - Обычный\n"); break;
            case AXOLOTL: printf("40%% - Обычный\n"); break;
            case DRAGON: printf("8%% - Редкий! 🎉\n"); break;
            case PHOENIX: printf("9%% - Редкий! 🎉\n"); break;
            case UNICORN: printf("3%% - УЛЬТРА РЕДКИЙ! 🌟\n"); break;
        }
        
        printf("\nЧто хочешь сделать?\n");
        printf("1 - ✅ Оставить этого питомца\n");
        printf("2 - 🔄 Потратить жетон для замены (осталось: %d)\n", tokens);
        printf("Выбор: ");
        
        int choice;
        scanf("%d", &choice);
        
        if (choice == 1) {
            choosing = 0;
            printf("✅ Ты выбрал %s!\n", type_names[chosen_pet.type]);
        } else if (choice == 2) {
            if (tokens > 0) {
                tokens--;
                printf("🔄 Использован 1 жетон. Осталось: %d\n", tokens);
                printf("🌀 Ищу нового питомца...\n");
                sleep(2);
            } else {
                printf("❌ У тебя закончились жетоны! Придется оставить этого питомца.\n");
                choosing = 0;
            }
        } else {
            printf("❌ Неверный выбор! Попробуй снова.\n");
        }
    }
    
    return chosen_pet;
}

// Функция замены питомца во время игры
int replace_pet(Pet *pet) {
    if (tokens <= 0) {
        printf("❌ У тебя нет жетонов для замены питомца!\n");
        return 0;
    }
    
    printf("\n🔄 Использовать жетон для замены питомца? (y/n): ");
    char choice;
    scanf(" %c", &choice);
    
    if (choice == 'y' || choice == 'Y') {
        tokens--;
        Pet new_pet = create_new_pet();
        
        printf("🎉 Выпал новый питомец: %s!\n", type_names[new_pet.type]);
        printf("Как назовешь нового питомца? ");
        scanf("%s", new_pet.name);
        
        *pet = new_pet;
        printf("🔮 %s появляется в магическом сиянии! Осталось жетонов: %d\n", pet->name, tokens);
        return 1;
    }
    return 0;
}

// Сохранение прогресса в виде "магического кода"
void save_progress(Pet *pet) {
    printf("\n🔮 Магический код для сохранения:\n");
    printf("===НАЧАЛО КОДА===\n");
    printf("%s|%d|%d|%d|%d|%d|%d|%d|%d|%d\n", 
           pet->name, pet->type, pet->stage, pet->age, 
           pet->evolution_points, pet->legendary_points,
           achievements.played_games, achievements.survived_storms, game_cycles, tokens);
    printf("===КОНЕЦ КОДА===\n");
    printf("Запиши этот код чтобы продолжить позже!\n");
}

// Отображение достижений
void show_achievements() {
    printf("\n🏆 Достижения:\n");
    printf("%s Первая эволюция\n", achievements.first_evolution ? "✅" : "❌");
    printf("%s Сыграно в игры: %d/10\n", achievements.played_games >= 10 ? "✅" : "❌", achievements.played_games);
    printf("%s Пережито бурь: %d/5\n", achievements.survived_storms >= 5 ? "✅" : "❌", achievements.survived_storms);
    printf("%s Достигнута мифическая форма\n", achievements.reached_mythical ? "✅" : "❌");
    printf("%s СТАНЬ ЛЕГЕНДОЙ!\n", achievements.reached_legendary ? "✅👑" : "❌");
}

// Специальные способности для легендарной стадии
void use_legendary_power(Pet *pet) {
    if(pet->stage != LEGENDARY) {
        printf("❌ Эта способность доступна только легендарным существам!\n");
        return;
    }
    
    printf("\n👑 ВЫБЕРИ СИЛУ ЛЕГЕНДЫ:\n");
    printf("1 - 🌟 Восстановить все показатели\n");
    printf("2 - 🔮 Создать магическую бурю\n");
    printf("3 - 💫 Поделиться мудростью (получить достижения)\n");
    
    char choice;
    scanf(" %c", &choice);
    
    switch(choice) {
        case '1':
            pet->hunger = 30;
            pet->happiness = 100;
            pet->energy = 100;
            pet->magic = 100;
            printf("🌈 Все показатели восстановлены силой легенды!\n");
            break;
        case '2':
            current_weather = MAGICAL;
            printf("🌪️ %s создаёт магическую бурю!\n", pet->name);
            break;
        case '3':
            achievements.played_games = 10;
            achievements.survived_storms = 5;
            printf("📚 %s делится древней мудростью! Достижения получены!\n", pet->name);
            break;
        default:
            printf("❌ Неверный выбор!\n");
    }
}

// Главный игровой цикл
int main() {
    srand(time(NULL));
    SetConsoleOutputCP(65001);
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    printf("🐉 Добро пожаловать в МАГИЧЕСКИЙ ТАМАГОЧИ!\n");
    printf("🎰 У тебя есть 10 жетонов для замены питомцев\n");
    printf("📊 Шансы выпадения:\n");
    printf("   🦁🐐🐍 Химера: 40%% (Обычный)\n");
    printf("   🦎 Аксолотль: 40%% (Обычный)\n");
    printf("   🐲 Дракон: 8%% (Редкий)\n");
    printf("   🐦 Феникс: 9%% (Редкий)\n");
    printf("   🦄 Единорог: 3%% (УЛЬТРА РЕДКИЙ)\n\n");
    
    // Выбор начального питомца
    Pet my_pet = choose_starting_pet();
    
    // Даем имя выбранному питомцу
    printf("\nКак назовешь своего %s? ", type_names[my_pet.type]);
    scanf("%s", my_pet.name);
    
    printf("\n🎉 Родился новый %s по имени %s!\n", type_names[my_pet.type], my_pet.name);
    printf("💡 Цель: достичь ЛЕГЕНДАРНОЙ стадии! (50 очков легенды после мифической формы)\n");
    printf("🔄 Осталось жетонов для замены: %d\n", tokens);
    
    while(my_pet.is_alive) {
        system("cls");
        
        // Обновление среды
        update_environment();
        apply_environment_effects(&my_pet);
        
        // Отображение питомца
        printf("\n╔══════════════════════════════════════════╗");
        printf("\n║           🐾 %-12s 🐾           ║", my_pet.name);
        printf("\n║         %-10s %-14s ║", type_names[my_pet.type], stage_names[my_pet.stage]);
        printf("\n╠══════════════════════════════════════════╣");
        
        // Статус бары
        printf("\n║ Голод:     [%-10s] %3d%% ║", 
               my_pet.hunger > 50 ? "██████████" : "----------", my_pet.hunger);
        printf("\n║ Настроение:[%-10s] %3d%% ║", 
               my_pet.happiness > 50 ? "██████████" : "----------", my_pet.happiness);
        printf("\n║ Энергия:  [%-10s] %3d%% ║", 
               my_pet.energy > 50 ? "██████████" : "----------", my_pet.energy);
        printf("\n║ Магия:    [%-10s] %3d%% ║", 
               my_pet.magic > 50 ? "██████████" : "----------", my_pet.magic);
        
        // Разные шкалы прогресса в зависимости от стадии
        if(my_pet.stage < MYTHICAL) {
            printf("\n║ Эволюция: [%-10s] %3d/30 ║", 
                   "██████████" + (10 - (int)(my_pet.evolution_points/3.0)), my_pet.evolution_points);
        } else if(my_pet.stage == MYTHICAL) {
            printf("\n║ Легенда:  [%-10s] %3d/50 ║", 
                   "██████████" + (10 - (int)(my_pet.legendary_points/5.0)), my_pet.legendary_points);
        } else {
            printf("\n║ 💥 ЛЕГЕНДА АКТИВНА!              ║");
        }
        
        printf("\n║ Возраст:  %4d циклов             ║", my_pet.age);
        printf("\n║ Жетоны:   %4d шт.                ║", tokens);
        printf("\n╚══════════════════════════════════════════╝");
        
        draw_pet_art(&my_pet);
        
        // Меню действий
        printf("\n🔮 Действия:");
        printf("\n1 - 🍖 Накормить");
        printf("\n2 - 🎮 Поиграть в магическую игру");
        printf("\n3 - 💤 Усыпить магическим сном");
        printf("\n4 - ✨ Тренировать магию");
        printf("\n5 - 🏆 Достижения");
        printf("\n6 - 💾 Сохранить магическим кодом");
        printf("\n7 - 🔄 ЗАМЕНИТЬ ПИТОМЦА (%d жетонов)", tokens);
        
        if(my_pet.stage < MYTHICAL) {
            printf("\n8 - 🎇 УСКОРИТЬ ЭВОЛЮЦИЮ (%d/30)", my_pet.evolution_points);
        } else if(my_pet.stage == MYTHICAL) {
            printf("\n8 - 🌟 НАКОПИТЬ СИЛу ЛЕГЕНДЫ (%d/50)", my_pet.legendary_points);
        } else {
            printf("\n8 - 👑 ИСПОЛЬЗОВАТЬ СИЛУ ЛЕГЕНДЫ");
        }
        
        printf("\nq - 🚪 Выйти");
        printf("\nВыбор: ");
        
        char choice;
        scanf(" %c", &choice);
        
        switch(choice) {
            case '1': 
                my_pet.hunger -= 25;
                my_pet.happiness += 10;
                if(my_pet.stage == MYTHICAL) my_pet.legendary_points += 2;
                printf("🍖 %s с удовольствием ест магическую еду!\n", my_pet.name);
                break;
                
            case '2': {
                printf("\nВыбери игру:\n1 - Магическая угадайка\n2 - Битва стихий\n");
                if(my_pet.stage >= MYTHICAL) {
                    printf("3 - 🔮 ВЕЛИКИЙ РИТУАЛ ЛЕГЕНДЫ (только для мифических+)\n");
                }
                
                int game_choice;
                scanf("%d", &game_choice);
                
                int result = 0;
                if(game_choice == 1) result = magic_guess_game();
                else if(game_choice == 2) result = element_battle();
                else if(game_choice == 3 && my_pet.stage >= MYTHICAL) result = legendary_ritual();
                else {
                    printf("❌ Неверный выбор!\n");
                    break;
                }
                
                if(result > 0) {
                    my_pet.happiness += 20;
                    my_pet.energy -= 10;
                    achievements.played_games++;
                    
                    if(my_pet.stage < MYTHICAL) {
                        my_pet.evolution_points += 5;
                    } else if(my_pet.stage == MYTHICAL) {
                        my_pet.legendary_points += (result == 3) ? 15 : 5;
                    }
                }
                break;
            }
                
            case '3':
                my_pet.energy += 30;
                my_pet.magic += 10;
                if(my_pet.stage == MYTHICAL) my_pet.legendary_points += 3;
                printf("💤 %s погружается в магический сон...\n", my_pet.name);
                break;
                
            case '4':
                my_pet.magic += 15;
                my_pet.energy -= 10;
                if(my_pet.stage < MYTHICAL) {
                    my_pet.evolution_points += 3;
                } else if(my_pet.stage == MYTHICAL) {
                    my_pet.legendary_points += 8;
                }
                printf("✨ %s тренирует магические способности!\n", my_pet.name);
                break;
                
            case '5':
                show_achievements();
                break;
                
            case '6':
                save_progress(&my_pet);
                break;
                
            case '7':
                replace_pet(&my_pet);
                break;
                
            case '8':
                if(my_pet.stage < MYTHICAL && my_pet.evolution_points >= 30) {
                    evolve_pet(&my_pet);
                } else if(my_pet.stage == MYTHICAL && my_pet.legendary_points >= 50) {
                    evolve_pet(&my_pet);
                } else if(my_pet.stage == LEGENDARY) {
                    use_legendary_power(&my_pet);
                } else {
                    printf("❌ Недостаточно очков для эволюции!\n");
                }
                break;
                
            case 'q':
                printf("👋 До новых встреч в магическом мире!\n");
                return 0;
        }
        
        // Обновление состояния
        my_pet.hunger += 3;
        my_pet.happiness -= 2;
        my_pet.energy -= 2;
        my_pet.age++;
        
        // Проверка жизнеспособности
        if(my_pet.hunger >= 100 || my_pet.happiness <= 0 || my_pet.energy <= 0) {
            my_pet.is_alive = 0;
        }
        
        // Автоматическая эволюция при достижении очков
        if((my_pet.stage < MYTHICAL && my_pet.evolution_points >= 30) ||
           (my_pet.stage == MYTHICAL && my_pet.legendary_points >= 50)) {
            evolve_pet(&my_pet);
        }
        
        sleep(2);
    }
    
    // Конец игры
    printf("\n💀 %s покинул этот мир...\n", my_pet.name);
    printf("🐾 Прожито: %d циклов\n", my_pet.age);
    printf("🏆 Достижения за жизнь:\n");
    show_achievements();
    
    if(achievements.reached_legendary) {
        printf("\n💥👑 НЕВЕРОЯТНО! Ты достиг ЛЕГЕНДАРНОЙ стадии!\n");
        printf("Твое имя будет вечно сиять в залах магических легенд!\n");
    }
    
    return 0;
}