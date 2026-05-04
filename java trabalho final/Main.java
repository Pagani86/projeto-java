import java.util.Scanner;

/**
 * Ponto de entrada do sistema.
 * Cria gerenciadores e mostra o menu principal.
 */
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        GerenciadorProdutos gp = new GerenciadorProdutos(sc);
        CadastroClientes cc = new CadastroClientes(sc);
        CadastroVendas cv = new CadastroVendas(gp, cc, sc);

        int opcao = -1;
        while (opcao != 0) {
            System.out.println("\n=== MENU PRINCIPAL ===");
            System.out.println("1 - Módulo Produtos");
            System.out.println("2 - Módulo Clientes");
            System.out.println("3 - Módulo Vendas");
            System.out.println("0 - Sair");
            System.out.print("Escolha: ");
            try {
                opcao = Integer.parseInt(sc.nextLine());
            } catch (Exception e) {
                System.out.println("Entrada inválida.");
                continue;
            }
            switch (opcao) {
                case 1: gp.menu(); break;
                case 2: cc.menu(); break;
                case 3: cv.menu(); break;
                case 0: System.out.println("Encerrando..."); break;
                default: System.out.println("Opção inválida.");
            }
        }

        sc.close();
    }
}
