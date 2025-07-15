/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:34:40 by kikiz             #+#    #+#             */
/*   Updated: 2025/05/27 16:12:18 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include "libft/libft.h"

/* Temel Veri Yapıları */
typedef struct s_dlist {
    void        *data;
    struct s_dlist *next;
    struct s_dlist *prev;
} t_dlist;

/* Dosya Yönlendirme Yapısı */
typedef struct s_redirect {
    int     std_in_backup;
    int     std_out_backup;
    int     redirect_active;
    int     heredoc_count;
    int     *heredoc_fds;
} t_redirect;

/* Sinyal Yönetimi */
typedef struct s_signals {
    int     sigint;
    int     sigquit;
    int     in_heredoc;
} t_signals;

/* Ortam Değişkenleri */
typedef struct s_env {
    t_dlist     *vars;          // Linked list olarak ortam değişkenleri
    char        **env_array;    // execve için düz array
    char        *oldpwd;
    char        *path;
    char        *pwd;
} t_env;

/* Parse İşlemleri */
typedef struct s_parser {
    char        *line;          // Ham input
    t_dlist     *tokens;        // Tokenize edilmiş liste
    char        ***commands;    // Pipe'a göre ayrılmış komutlar
    char        ***args;        // Argümanlar (execve formatında)
    char        ***expanded;    // Genişletilmiş argümanlar
    int         cmd_count;      // Komut sayısı
} t_parser;

/* Komut Yürütme */
typedef struct s_executor {
    int         pipe_count;
    int         *pipe_fds;      // Pipe file descriptor'ları
    int         *pids;          // Çocuk proses PID'leri
    int         last_status;    // Son çıkış durumu
    int         is_parent;      // Ana proses mi kontrolü
    int         exit_requested; // Çıkış isteği flag'i
} t_executor;

/* Tarihçe ve Oturum Yönetimi */
typedef struct s_session {
    t_dlist     *history;       // Komut geçmişi
    int         hist_fd;        // History dosya tanımlayıcı
    char        *hist_path;     // History dosya yolu
} t_session;

/* Ana Minishell Yapısı */
typedef struct s_minishell {
    t_env       env;
    t_parser    parser;
    t_executor  executor;
    t_redirect redirect;
    t_signals  signals;
    t_session  session;
    
    /* Global Ayarlar */
    int         interactive;    // Etkileşimli modda mı
    int         debug_mode;     // Debug çıktıları aktif mi
    char        *name;         // Shell adı (örn: "minishell")
} t_minishell;

#endif