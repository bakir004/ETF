using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace ooadepazar.Migrations
{
    /// <inheritdoc />
    public partial class hajjealelfelaaaaaaaaa : Migration
    {
        /// <inheritdoc />
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.AddColumn<int>(
                name: "PraceniID",
                table: "Pracenje",
                type: "int",
                nullable: true);

            migrationBuilder.AddColumn<int>(
                name: "PraceniKorisnikID",
                table: "Pracenje",
                type: "int",
                nullable: true);

            migrationBuilder.AddColumn<int>(
                name: "PratilacID",
                table: "Pracenje",
                type: "int",
                nullable: true);

            migrationBuilder.AddColumn<int>(
                name: "PratilacKorisnikID",
                table: "Pracenje",
                type: "int",
                nullable: true);

            migrationBuilder.CreateIndex(
                name: "IX_Pracenje_PraceniKorisnikID",
                table: "Pracenje",
                column: "PraceniKorisnikID");

            migrationBuilder.CreateIndex(
                name: "IX_Pracenje_PratilacKorisnikID",
                table: "Pracenje",
                column: "PratilacKorisnikID");

            migrationBuilder.AddForeignKey(
                name: "FK_Pracenje_Korisnik_PraceniKorisnikID",
                table: "Pracenje",
                column: "PraceniKorisnikID",
                principalTable: "Korisnik",
                principalColumn: "ID");

            migrationBuilder.AddForeignKey(
                name: "FK_Pracenje_Korisnik_PratilacKorisnikID",
                table: "Pracenje",
                column: "PratilacKorisnikID",
                principalTable: "Korisnik",
                principalColumn: "ID");
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropForeignKey(
                name: "FK_Pracenje_Korisnik_PraceniKorisnikID",
                table: "Pracenje");

            migrationBuilder.DropForeignKey(
                name: "FK_Pracenje_Korisnik_PratilacKorisnikID",
                table: "Pracenje");

            migrationBuilder.DropIndex(
                name: "IX_Pracenje_PraceniKorisnikID",
                table: "Pracenje");

            migrationBuilder.DropIndex(
                name: "IX_Pracenje_PratilacKorisnikID",
                table: "Pracenje");

            migrationBuilder.DropColumn(
                name: "PraceniID",
                table: "Pracenje");

            migrationBuilder.DropColumn(
                name: "PraceniKorisnikID",
                table: "Pracenje");

            migrationBuilder.DropColumn(
                name: "PratilacID",
                table: "Pracenje");

            migrationBuilder.DropColumn(
                name: "PratilacKorisnikID",
                table: "Pracenje");
        }
    }
}
