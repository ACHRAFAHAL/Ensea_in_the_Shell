# Ensea in the Shell

Micro shell project


### Question 1 
Afficher un message de bienvenue au lancement du shell et un prompt simple `enseash %` qui attend les commandes de l'utilisateur.

On utilise pour cela la fonction `write()` :

```c
write(int fd, const void *buf, size_t count);
```
On test ce code et on obtient le message d’accueil, suivi du prompt enseash % :

![Q1](/screenshots/Capture%20d'écran%202025-12-10%20164020.png)

### Question 2

On veut creer la loop REPL (Read-Eval-Print Loop) pour lire les commandes saisies par l'utilisateur et rtourne au prompt après l'exécution. 

Pour cela on utilise les fonctons suivantes : 

La fonction `read_command()` qui utilise :
```c
read(int fd, void *buf, size_t count);
```
la fonction retourne le nombre d'octets lus, -1 en cas d'erreur, pour lire la commande entré par l'utlisateur et enlevève le \n a la fin pour reconaitre la commande lors de l'execution. Puis on ajoute le \0 qui marque la fin de la commande dans le buffer.

La fonction `execute_command()` qui utilise :
```c
fork(void);
```
Pour crée un nouveau processus en copiant du processus parent et le processus fils sera remplacé. Si on utilise `execvp()` directement, on quite le shell car `execvp()` remplace complètement le processus.

```c
execlp(const char *file, const char *arg, (char  *) NULL);
```
Remplace le processus actuel par le programme a executé
- `file` : Nom du programme à exécuter (ex: `"date"`)
- `argv` : Tableau d'arguments contenant le nom du programme et ses paramètres, terminé par `NULL`


```c
waitpid(pid_t pid, int *status, int options);
```
Pour attendre la fin du processus fils et recuperer sa sortie si necessaire

dans le main on ajoute :
```c
if (strlen(command) == 0) {
            continue;
        }
```
pour continuer quand la comande est vide " ".

![Q2](/screenshots/Capture%20d'écran%202025-12-10%20164048.png)
![Q22](/screenshots/Capture%20d'écran%202025-12-10%20164030.png)

### Question 3

Gestion de la sortie du shell avec la commande "exit" ou `<Ctrl>+d`.

Pour cela on ajoute les fonctions suivantes :

La fonction `is_exit_command()` qui utilise :
```c
strncmp(const char *s1, const char *s2, size_t n);
```
pour comparer la commande entrée avec "exit". On utilise `strncmp()` au lieu de `strcmp()` pour limiter le nombre de caractères comparés et éviter les 'out of ranges'.

Dans le main : 
```c
bytes_read = read_command(command, bufsize);

if (bytes_read <= 0) { 
    display("\n");
    display(exit_msg);
    break;
}
```
 `Ctrl+D` => la fonction `read()` retourne 0. On affiche alors le message de sortie et on quitte la boucle.

![Q3](/screenshots/Capture%20d'écran%202025-12-10%20165320.png)
![Q3](/screenshots/Capture%20d'écran%202025-12-10%20165337.png)

### Question 4

Affichage du code de retour (ou le signal) de la commande précédente dans le prompt.
on utilise les macros suivantes :

```c
WIFEXITED(status)    
WEXITSTATUS(status)  
WIFSIGNALED(status)  
WTERMSIG(status)    
```
Dans la fonction `execute_command()`, après `waitpid()`, on vérifie si le processus s'est terminé normalement avec `WIFEXITED()` puis on récupère le code de retour avec `WEXITSTATUS()`. Sinon, si le processus a été terminé par un signal avec `WIFSIGNALED()`, on récupère le numéro du signal avec `WTERMSIG()`.

Pour faciliter les choses on se permet d'utliser la fonction :
```c
sprintf(char *str, const char *format, ...);
```

![Q4](/screenshots/Capture%20d'écran%202025-12-17%20142822.png)

le 127 est le code standart pour l'erreur "Command not found" (Tous les codes d'erreurs : https://snowcodes.hashnode.dev/all-the-exit-codes-and-status-in-c ). Pour verifier le retour de signal, on tue le processus pendant son execution. 


![Q4](/screenshots/Capture%20d'écran%202025-12-17%20143427.png)

### Question 5

Mesurer le temps d'exécution de la commande et l'afficher dans le prompt.
On utilise la fonction :
```c
clock_gettime(clockid_t clk_id, struct timespec *tp);
```
avec `CLOCK_MONOTONIC` pour obtenir un temps monotone.

La structure `timespec` contient :
```c
struct timespec {
    time_t tv_sec;   // secondes
    long tv_nsec;    // nanosecondes
};
```
L'appelle est fait avant et après `waitpid()` puis on calcule la différence pour mesurer le temps d'exécution en millisecondes.

![Q5](/screenshots/Capture%20d'écran%202025-12-17%20142031.png)

### Question 6

Gestion des commandes complexes avec arguments.
Pour les questions precedentes, on utilisait `execlp()` qui ne permet d'exécuter que des commandes sans arguments. Pour gérer les arguments, on doit parse la commande pour séparer le nom du programme et ses arguments.

Pour faciliter, on se permet d'utiliser la fonction :
```c
strtok(char *str, const char *delim);
```
pour découper la chaîne de commande en tokens séparés par des espaces, puis avec la fonction `parse_command()`, on crée un tableau d'arguments.

Dans `execute_command()`, on utilise `execvp(...)` au lieu de `execlp(...)`, et on passe le tableau d'arguments terminé par `NULL`.

![Q6](/screenshots/Capture%20d'écran%202025-12-17%20163852.png)

### Question 7

Gestion des redirections d'entrée/sortie avec `<` et `>`.
On modifie la fonction `parse_command()` pour détecter les symboles `<` et `>` dans la commande et extraire les noms de fichiers associés

Les fonctions système utilisées pour gerrer ces redirection sont :

```c
open(const char *pathname, int flags, mode_t mode);
```
```c
dup2(int oldfd, int newfd);
```
```c
close(int fd);
```

Dans le processus fils, on applique les redirections si nécessaire.

![Q7](/screenshots/Capture%20d'écran%202025-12-17%20171936.png)

## Organisation et Factorisation du Code

Pour améliorer la maintenabilité du code, on a factorisé les différentes fonctionnalités en modules distincts :

- `config.h` - les constants (welcome, Promt, bufsize, max_arg, etc.)
- `display.h / display.c` - les functions d'affichage
- `shell.h / shell.c` - les functions core de la shell 
- `main.c - REPL loop`
