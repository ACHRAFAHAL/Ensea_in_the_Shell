# Ensea in the Shell

Micro shell project


### Question 1 
Afficher un message de bienvenue au lancement du shell et un prompt simple `enseash %` qui attend les commandes de l'utilisateur.

On utilise pour cela la fonction `write()` :

```c
ssize_t write(int fd, const void *buf, size_t count);
```
- `fd` : Descripteur de fichier (ici `STDOUT_FILENO` = 1 pour l'écran)
- `buf` : Pointeur vers les données à écrire
- `count` : Nombre d'octets à écrire

### Question 2

On veut creer la loop REPL (Read-Eval-Print Loop) pour lire les commandes saisies par l'utilisateur et rtourne au prompt après l'exécution. 

Pour cela on utilise les fonctons suivantes : 

La fonction `read_command()` qui utilise :
```c
ssize_t read(int fd, void *buf, size_t count);
```
- `fd` : Descripteur de fichier (ici `STDIN_FILENO` = 0 pour le clavier)
- `buf` : Buffer où stocker les données lues
- `count` : Nombre maximum d'octets à lire
- **Retour** : Nombre d'octets lus, -1 en cas d'erreur

pour lire la commande entré par l'utlisateur et enlevève le \n a la fin pour reconaitre la commande lors de l'execution. Puis on ajoute le \0 qui marque la fin de la commande dans le buffer.



La fonction `execute_command()` qui utilise 
```c
pid_t fork(void);
```
Crée un nouveau processus en copiant du processus parent et le processus fils sera remplacé. Si on utilise `execvp()` directement, on quite le shell car `execvp()` remplace complètement le processus.

```c
int execvp(const char *file, char *const argv[]);
```
Remplace le processus actuel par un nouveau programme
- `file` : Nom du programme à exécuter (ex: `"date"`)
- `argv` : Tableau d'arguments contenant le nom du programme et ses paramètres, terminé par `NULL`


```c
pid_t waitpid(pid_t pid, int *status, int options);
```

- `status` : Pointeur pour stocker le statut de sortie
- `options` : 0 pour attente bloquante
- **Retour** : PID du processus terminé, ou -1 en cas d'erreur