import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

/**
 * Gerencia o cadastro de clientes.
 */
public class CadastroClientes {
    private List<Cliente> clientes;
    private Scanner sc;

    public CadastroClientes(Scanner sc) {
        this.clientes = new ArrayList<Cliente>();
        this.sc = sc;
    }

    public void menu() {
        int op = -1;
        while (op != 0) {
            System.out.println("\n--- MÓDULO CLIENTES ---");
            System.out.println("1 - Incluir Cliente");
            System.out.println("2 - Listar Clientes");
            System.out.println("3 - Buscar Cliente por nome");
            System.out.println("0 - Voltar");
            System.out.print("Escolha: ");
            try { op = Integer.parseInt(sc.nextLine()); } catch (Exception e) { System.out.println("Entrada inválida."); continue; }

            switch (op) {
                case 1: incluir(); break;
                case 2: listar(); break;
                case 3: buscarPorNome(); break;
                case 0: break;
                default: System.out.println("Opção inválida.");
            }
        }
    }

    private void incluir() {
        System.out.print("Nome: ");
        String nome = sc.nextLine().trim();
        if (nome.isEmpty()) { System.out.println("Nome vazio."); return; }
        System.out.print("Documento (CPF/CNPJ) ou ENTER para pular: ");
        String doc = sc.nextLine().trim();
        if (doc.isEmpty()) doc = null;
        Cliente c = new Cliente(nome, doc);
        clientes.add(c);
        System.out.println("Cliente cadastrado: " + c.toString());
    }

    private void listar() {
        if (clientes.isEmpty()) { System.out.println("Nenhum cliente cadastrado."); return; }
        System.out.println("\n--- LISTA DE CLIENTES ---");
        for (Cliente c : clientes) System.out.println(c.toString());
    }

    private void buscarPorNome() {
        System.out.print("Digite parte do nome: ");
        String termo = sc.nextLine().trim().toLowerCase();
        boolean achou = false;
        for (Cliente c : clientes) {
            if (c.getNome().toLowerCase().contains(termo)) {
                c.exibir();
                System.out.println("----");
                achou = true;
            }
        }
        if (!achou) System.out.println("Nenhum cliente encontrado.");
    }

    // seleção por índice para uso em vendas (retorna null se não houver ou inválido)
    public Cliente selecionarCliente() {
        if (clientes.isEmpty()) { System.out.println("Nenhum cliente cadastrado."); return null; }
        System.out.println("\nSelecione cliente (digite ID):");
        listar();
        System.out.print("ID: ");
        int id;
        try { id = Integer.parseInt(sc.nextLine()); } catch (Exception e) { System.out.println("ID inválido."); return null; }
        for (Cliente c : clientes) if (c.getId() == id) return c;
        System.out.println("Cliente não encontrado.");
        return null;
    }
}
