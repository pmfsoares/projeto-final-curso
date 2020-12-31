-- phpMyAdmin SQL Dump
-- version 5.0.4
-- https://www.phpmyadmin.net/
--
-- Host: localhost
-- Tempo de geração: 03-Dez-2020 às 15:14
-- Versão do servidor: 10.5.8-MariaDB
-- versão do PHP: 7.4.13

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Banco de dados: `projeto`
--

-- --------------------------------------------------------

--
-- Estrutura da tabela `recordings`
--

CREATE TABLE `recordings` (
  `id` int(11) NOT NULL,
  `sensor_mac` varchar(17) COLLATE utf8mb4_unicode_ci NOT NULL,
  `sensor_descriptor` varchar(30) CHARACTER SET utf8 DEFAULT NULL,
  `data` longblob DEFAULT NULL,
  `timestamp` datetime DEFAULT NULL,
  `sample_rate` int(11) DEFAULT NULL,
  `sample_size` int(11) DEFAULT NULL,
  `filename` varchar(50) COLLATE utf8mb4_unicode_ci DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- --------------------------------------------------------

--
-- Estrutura da tabela `sensors`
--

CREATE TABLE `sensors` (
  `id` int(11) NOT NULL,
  `sensor_descriptor` varchar(30) CHARACTER SET utf8 DEFAULT NULL,
  `model` varchar(30) CHARACTER SET utf8 DEFAULT NULL,
  `coord_long` double DEFAULT NULL,
  `coord_lat` double DEFAULT NULL,
  `status` int(11) DEFAULT NULL,
  `sample_rate` int(11) DEFAULT NULL,
  `sample_size` int(11) DEFAULT NULL,
  `sensor_mac` varchar(17) COLLATE utf8mb4_unicode_ci NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

--
-- Extraindo dados da tabela `sensors`
--

INSERT INTO `sensors` (`id`, `sensor_descriptor`, `model`, `coord_long`, `coord_lat`, `status`, `sample_rate`, `sample_size`, `sensor_mac`) VALUES
(19, 'd1', 'nodemcu ', -7.44208065265089, 39.26904235164174, 1, 8000, 1600, '7B:36:BD:1E:ED:17'),
(20, 'd2', 'nodemcu ', -7.442237853501268, 39.26879511102456, 3, 16000, 4000, '3D:81:D2:D5:77:14'),
(21, 'd3', 'nodemcu ', -7.441763764011412, 39.26894115348663, 3, 16000, 4000, 'D7:16:56:9F:FA:3B'),
(22, 'd4', 'nodemcu ', -7.441916076243379, 39.26867562320613, 3, 16000, 1500, '71:C3:C2:A6:88:CE');

-- --------------------------------------------------------

--
-- Estrutura da tabela `signup_tokens`
--

CREATE TABLE `signup_tokens` (
  `id` int(11) NOT NULL,
  `token` varchar(20) CHARACTER SET utf8 DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

--
-- Extraindo dados da tabela `signup_tokens`
--

INSERT INTO `signup_tokens` (`id`, `token`) VALUES
(1, 'sudo_admin_projeto'),
(2, 'orientador_projeto'),
(3, 'user_comum');

-- --------------------------------------------------------

--
-- Estrutura da tabela `status_type`
--

CREATE TABLE `status_type` (
  `id` int(11) NOT NULL,
  `description` varchar(50) CHARACTER SET utf8 DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

--
-- Extraindo dados da tabela `status_type`
--

INSERT INTO `status_type` (`id`, `description`) VALUES
(1, 'validated'),
(2, 'registered'),
(3, 'not registered');

-- --------------------------------------------------------

--
-- Estrutura da tabela `users`
--

CREATE TABLE `users` (
  `id` int(11) NOT NULL,
  `username` varchar(50) CHARACTER SET utf8 DEFAULT NULL,
  `name` varchar(50) CHARACTER SET utf8 DEFAULT NULL,
  `email` varchar(50) CHARACTER SET utf8 DEFAULT NULL,
  `phone` varchar(9) CHARACTER SET utf8 DEFAULT NULL,
  `user_type` int(11) DEFAULT NULL,
  `hash_pwd` varchar(100) CHARACTER SET utf8 DEFAULT NULL,
  `signup_token` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

--
-- Extraindo dados da tabela `users`
--

INSERT INTO `users` (`id`, `username`, `name`, `email`, `phone`, `user_type`, `hash_pwd`, `signup_token`) VALUES
(4, 'teste_hash', 'teste user', 'teste@gmail.com', '123', 1, '$2y$10$keZvL5vqDelZxr2zuIWIyu4dOHnHVH.vd73y9e6i9vLzFDsSi.qp2', 2),
(6, 'admin', 'Administrador', 'admin', '123', 1, '$2y$10$Ry9upMzNayuT7Od3Wf4rBOH0zEbUkBL8yHYEBIsRW8kS0vPX5dQLi', 1);

-- --------------------------------------------------------

--
-- Estrutura da tabela `users_type`
--

CREATE TABLE `users_type` (
  `id` int(11) NOT NULL,
  `description` varchar(20) CHARACTER SET utf8 DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

--
-- Extraindo dados da tabela `users_type`
--

INSERT INTO `users_type` (`id`, `description`) VALUES
(1, 'administrador'),
(2, 'utilizador'),
(3, 'agente');

--
-- Índices para tabelas despejadas
--

--
-- Índices para tabela `recordings`
--
ALTER TABLE `recordings`
  ADD PRIMARY KEY (`id`);

--
-- Índices para tabela `sensors`
--
ALTER TABLE `sensors`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `sensor_mac` (`sensor_mac`),
  ADD KEY `status` (`status`);

--
-- Índices para tabela `signup_tokens`
--
ALTER TABLE `signup_tokens`
  ADD PRIMARY KEY (`id`);

--
-- Índices para tabela `status_type`
--
ALTER TABLE `status_type`
  ADD PRIMARY KEY (`id`);

--
-- Índices para tabela `users`
--
ALTER TABLE `users`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `username` (`username`),
  ADD KEY `user_type` (`user_type`),
  ADD KEY `signup_token` (`signup_token`);

--
-- Índices para tabela `users_type`
--
ALTER TABLE `users_type`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT de tabelas despejadas
--

--
-- AUTO_INCREMENT de tabela `recordings`
--
ALTER TABLE `recordings`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT de tabela `sensors`
--
ALTER TABLE `sensors`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=23;

--
-- AUTO_INCREMENT de tabela `signup_tokens`
--
ALTER TABLE `signup_tokens`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;

--
-- AUTO_INCREMENT de tabela `status_type`
--
ALTER TABLE `status_type`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=4;

--
-- AUTO_INCREMENT de tabela `users`
--
ALTER TABLE `users`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=7;

--
-- AUTO_INCREMENT de tabela `users_type`
--
ALTER TABLE `users_type`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=4;

--
-- Restrições para despejos de tabelas
--

--
-- Limitadores para a tabela `sensors`
--
ALTER TABLE `sensors`
  ADD CONSTRAINT `sensors_ibfk_1` FOREIGN KEY (`status`) REFERENCES `status_type` (`id`);

--
-- Limitadores para a tabela `users`
--
ALTER TABLE `users`
  ADD CONSTRAINT `users_ibfk_1` FOREIGN KEY (`user_type`) REFERENCES `users_type` (`id`),
  ADD CONSTRAINT `users_ibfk_2` FOREIGN KEY (`signup_token`) REFERENCES `signup_tokens` (`id`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
