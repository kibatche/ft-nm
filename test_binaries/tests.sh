#!/bin/bash

# Définition des dossiers à parcourir
DIRS=("bin" "err" "lib" "obj")

# Pour chaque dossier
for DIR in "${DIRS[@]}"; do
    if [ -d "$DIR" ]; then
        echo "=== Test des fichiers ELF dans $DIR ==="

        # Liste des fichiers ELF dans le dossier
        FILES=()
        for FILE in "$DIR"/*; do
            if [ -f "$FILE" ]; then
                # Vérifier si c'est un fichier ELF
                if file "$FILE"; then
                    FILES+=("$FILE")

                    # Tester avec ft_nm_debug et nm
                    echo "---- Test de $FILE ----"
                    ./ft_nm_debug "$FILE" > "${FILE}.ft_nm"
                    nm "$FILE" > "${FILE}.nm"

                    # Comparer les sorties
                    diff -u "${FILE}.nm" "${FILE}.ft_nm" || echo "⚠ Différences détectées pour $FILE"
                fi
            fi
        done

        # Exécuter tous les ELF en une seule commande si au moins un ELF a été trouvé
        if [ ${#FILES[@]} -gt 0 ]; then
            echo "---- Test de tous les ELF du dossier $DIR ----"
            ./ft_nm_debug "${FILES[@]}" > "${FILE}.big.ft_nm"
            nm "${FILES[@]}" > "${FILE}.big.ft_nm"
        fi
    else
        echo "⚠ Le dossier $DIR n'existe pas."
    fi
done
